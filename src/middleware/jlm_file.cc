#include <fcntl.h>
#include <jl_core.h>
#include <jlm_file.h>
#include <unistd.h>

void FileMiddleware::ProcessInit() {
    /** 检查文件 */
    JudgeLightData *cur = jl_cycle->datas;
    while (cur) {
        if (access(cur->input_filepath, F_OK) != 0 ||
            access(cur->output_filepath, F_OK) != 0) {  // 文件不存在
            Exit(FILE_NOT_FOUND_ERROR);
        }
        if (access(cur->input_filepath, R_OK) != 0 ||
            access(cur->output_filepath, R_OK) != 0) {  // 文件不可读
            Exit(FILE_NOT_PERMISSION_ERROR);
        }
        cur = cur->next;
    }
}