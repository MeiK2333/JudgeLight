#include <dirent.h>
#include <jl_core.h>
#include <jlm_file.h>

void FileMiddleware::ProcessInit() {
    JudgeLightData *cur = jl_cycle->datas;
    while (cur) {
        /** 检查文件 */
        if (access(cur->input_filepath, F_OK) != 0 ||
            access(cur->output_filepath, F_OK) != 0) {  // 文件不存在
            Exit(FILE_NOT_FOUND_ERROR);
        }
        if (access(cur->input_filepath, R_OK) != 0 ||
            access(cur->output_filepath, R_OK) != 0) {  // 文件不可读
            Exit(FILE_NOT_PERMISSION_ERROR);
        }

        /** 打开文件，将描述符赋值给配置项 */
        cur->input_fd = open(cur->input_filepath, O_RDONLY);
        cur->output_fd = open(cur->output_filepath, O_RDONLY);

        cur = cur->next;
    }

    jl_cycle->temp_dir = new char[18];
    jl_cycle->temp_dir[0] = 'J';
    jl_cycle->temp_dir[1] = 'u';
    jl_cycle->temp_dir[2] = 'd';
    jl_cycle->temp_dir[3] = 'g';
    jl_cycle->temp_dir[4] = 'e';
    jl_cycle->temp_dir[5] = 'L';
    jl_cycle->temp_dir[6] = 'i';
    jl_cycle->temp_dir[7] = 'g';
    jl_cycle->temp_dir[8] = 'h';
    jl_cycle->temp_dir[9] = 't';
    jl_cycle->temp_dir[10] = '-';
    jl_cycle->temp_dir[11] = 'X';
    jl_cycle->temp_dir[12] = 'X';
    jl_cycle->temp_dir[13] = 'X';
    jl_cycle->temp_dir[14] = 'X';
    jl_cycle->temp_dir[15] = 'X';
    jl_cycle->temp_dir[16] = 'X';
    jl_cycle->temp_dir[17] = '\0';

    /** 创建临时文件夹，格式为 "JudgeLight-XXXXXX"，XXXXXX 被替换为随机字符串 */
    if (mkdtemp(jl_cycle->temp_dir) == nullptr) {
        Exit(FILE_CREATE_DIR_ERROR);
    }
    /** 切换工作目录 */
    if (chdir(jl_cycle->temp_dir) != 0) {
        Exit(FILE_CHDIR_ERROR);
    }
}

void FileMiddleware::ProcessExit() {
    /** 讲道理不会出现错误（因为你刚刚过来，没理由回不去） */
    if (chdir("../") != 0) {
        cout << "You will never arrive here" << endl;
    }
    remove_dir(jl_cycle->temp_dir);
}

/**
 * 递归删除非空文件夹
 * */
int remove_dir(const char *dir) {
    char dir_name[260];
    DIR *dirp;
    struct dirent *dp;
    struct stat dir_stat;

    if (S_ISREG(dir_stat.st_mode)) {  // 普通文件删除掉
        remove(dir);
    } else if (S_ISDIR(dir_stat.st_mode)) {  // 文件夹递归删除子文件，直到为空
        dirp = opendir(dir);
        while ((dp = readdir(dirp)) != NULL) {
            if ((0 == strcmp(".", dp->d_name)) ||
                (0 == strcmp("..", dp->d_name))) {
                continue;
            }
            sprintf(dir_name, "%s/%s", dir, dp->d_name);
            remove_dir(dir_name);
        }
        closedir(dirp);
        rmdir(dir);
    }
    return 0;
}