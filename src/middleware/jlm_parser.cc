#include <jl_core.h>
#include <jl_data.h>
#include <jlm_parser.h>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

using namespace std;

ParserStringCode Hashit(string const& in) {
    if (in == "time_limit") return TimeLimitCode;
    if (in == "memory_limit") return MemoryLimitCode;
    if (in == "code") return CodeCode;
    if (in == "code_length") return CodeLengthCode;
    if (in == "data") return DataCode;
    if (in == "filename") return FilenameCode;
    if (in == "compile_args") return CompileArgsCode;
    if (in == "run_args") return RunArgsCode;
    if (in == "done") return DoneCode;
    return UnknownCode;
}

void ParserMiddleware::ProcessInit() {
    string key;
    bool has_next = true;

    while (has_next) {
        cin >> key;
        switch (Hashit(key)) {
            case TimeLimitCode:
                cin >> jl_cycle->time_limit;
                break;
            case MemoryLimitCode:
                cin >> jl_cycle->memory_limit;
                break;
            case CodeLengthCode:
                cin >> jl_cycle->code_length;
                break;
            case DataCode: {
                JudgeLightData* data = new JudgeLightData();
                cin >> data->input_filepath;
                cin >> data->output_filepath;
                jl_cycle->PushData(data);
                break;
            }
            case CodeCode: {
                char* code_buffer = new char[jl_cycle->code_length + 1];
                int cur = 0;
                while (cur < jl_cycle->code_length) {
                    cin.get(code_buffer[cur++]);
                }
                code_buffer[cur] = '\0';
                jl_cycle->code = code_buffer;
                delete[] code_buffer;
                break;
            }
            case FilenameCode:
                cin >> jl_cycle->filename;
                break;
            case CompileArgsCode: {
                string temp_args;
                getline(cin, temp_args);  // 消除行末的回车符
                getline(cin, temp_args);

                // 以空格分割字符串
                istringstream iss(temp_args);
                vector<string> results((istream_iterator<string>(iss)),
                                       istream_iterator<string>());

                // 依次将字符串赋值给配置
                int i, len = results.size();
                jl_cycle->compile_args = new const char*[len + 1];
                for (i = 0; i < len; i++) {
                    jl_cycle->compile_args[i] = results[i].c_str();
                }
                jl_cycle->compile_args[i] = nullptr;

                break;
            }
            case RunArgsCode: {
                string temp_args;
                getline(cin, temp_args);  // 消除行末的回车符
                getline(cin, temp_args);

                istringstream iss(temp_args);
                vector<string> results((istream_iterator<string>(iss)),
                                       istream_iterator<string>());

                int i, len = results.size();
                jl_cycle->run_args = new const char*[len + 1];
                for (i = 0; i < len; i++) {
                    jl_cycle->run_args[i] = results[i].c_str();
                }
                jl_cycle->run_args[i] = nullptr;

                break;
            }
            case DoneCode:
                has_next = false;
                break;
            case UnknownCode:
                Exit(PARSER_UNKNOWN_ARGS);
                break;
        }
    }
}
