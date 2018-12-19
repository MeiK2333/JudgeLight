#include <ctype.h>
#include <jlm_result.h>
#include <sys/mman.h>

void ResultMiddleware::AllRunBefore() {
    /** 为每组数据创建 result */
    jl_cycle->results = new JudgeLightResult[jl_cycle->run_count + 1];
}

void ResultMiddleware::RunResult(int cnt, struct rusage &ru, int &status) {
    JudgeLightResult rst;

    /** 计算时间与内存占用 */
    rst.time_used = ru.ru_utime.tv_sec * 1000 + ru.ru_utime.tv_usec / 1000 +
                    ru.ru_stime.tv_sec * 1000 + ru.ru_stime.tv_usec / 1000;
    rst.memory_used = ru.ru_maxrss;

    if (WIFSIGNALED(status)) {  // 出现异常
        /* 获得退出原因 */
        switch (WTERMSIG(status)) {
            case SIGSEGV:
                if (rst.memory_used > jl_cycle->memory_limit) {
                    rst.result = MemoryLimitExceeded;
                } else {
                    rst.result = RuntimeError;
                }
                break;
            case SIGALRM:
            case SIGXCPU:
                rst.result = TimeLimitExceeded;
                break;
            default:
                rst.result = RuntimeError;
                break;
        }
        rst.signum = WTERMSIG(status);
        jl_cycle->run_error = true;
    } else {  // 未出现异常
        if (rst.time_used > jl_cycle->time_limit) {
            rst.result = TimeLimitExceeded;
        } else if (rst.memory_used > jl_cycle->memory_limit) {
            rst.result = MemoryLimitExceeded;
        } else {
            rst.result = DiffData(cnt);
        }
    }
    jl_cycle->run_error = rst.result != Accepted ? true : false;
    jl_cycle->results[cnt] = rst;

    // TODO
    // 输出本组数据的结果
    // 便于实时查看进度
}

// TODO
// 当所有数据运行结束后
// 计算最终结果并输出

int DiffData(int cnt) {
    JudgeLightData *data = jl_cycle->GetData(cnt);
    off_t temp_len, output_len;
    char *temp, *output;

    /** 计算文件大小 */
    output_len = lseek(data->output_fd, 0, SEEK_END);
    temp_len = lseek(data->temp_fd, 0, SEEK_END);

    if (output_len == -1 || temp_len == -1) {
        Exit(LSEEK_ERROR);
    }

    /** 重置文件偏移量 */
    if (lseek(data->output_fd, 0, SEEK_SET) == -1 ||
        lseek(data->temp_fd, 0, SEEK_SET) == -1) {
        Exit(LSEEK_ERROR);
    }

    /** 设置 mmap */
    if ((output = (char *)mmap(NULL, output_len, PROT_READ | PROT_WRITE,
                               MAP_PRIVATE, data->output_fd, 0)) ==
        MAP_FAILED) {
        Exit(MMAP_ERROR);
    }
    if ((temp = (char *)mmap(NULL, temp_len, PROT_READ | PROT_WRITE,
                             MAP_PRIVATE, data->temp_fd, 0)) == MAP_FAILED) {
        munmap(output, output_len);
        Exit(MMAP_ERROR);
    }

    int res = Accepted;
    bool pe = false;
    const char *output_cur, *temp_cur, *output_end, *temp_end;
    output_cur = output;
    output_end = output + output_len;
    temp_cur = temp;
    temp_end = temp + temp_len;

    while ((output_cur < output_end) && (temp_cur < temp_end)) {
        /** 如果用户输出与答案文件在某个位置不相同 */
        if (*output_cur != *temp_cur) {
            bool output_space = isspace(*output_cur);
            bool temp_space = isspace(*temp_cur);
            /** 如果是空白符的差别，则标记为 pe，继续向下处理 */
            if (output_space) {
                pe = true;
                output_cur++;
            }
            if (temp_space) {
                pe = true;
                temp_cur++;
            }
            /** 如果不是空白符不同，则标记为 wa，结束对比 */
            if (!output_space && !temp_space) {
                res = WrongAnswer;
                goto RESULTED;
            }
        } else {
            output_cur++;
            temp_cur++;
        }
    }

    /** 清理掉末尾的空白符 */
    while (output_cur < output_end && isspace(*output_cur)) {
        output_cur++;
    }
    while (temp_cur < temp_end && isspace(*temp_cur)) {
        temp_cur++;
    }

    /** 
     * 如果两者没有同时结束，则说明答案错误
     * 如果从这里错误，说明两者有包含关系
     * 如果后续需要有其他提示，可以从这里搞一下
     * */
    if ((output_cur != output_end) || (temp_cur != temp_end)) {
        res = WrongAnswer;
        goto RESULTED;
    }

RESULTED:
    munmap(temp, temp_len);
    munmap(output, output_len);

    /** 如果出现 PE 且过程中没有其他错误 */
    if (res == Accepted && pe) {
        res = pe;
    }

    return res;
}
