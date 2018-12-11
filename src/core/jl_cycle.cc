#include <jl_cycle.h>

void JudgeLightCycle::PushData(JudgeLightData* data) {
    JudgeLightData* head = this->datas;
    if (head == nullptr) {
        this->datas = data;
        return;
    }
    while (head->next) {
        head = head->next;
    }
    head->next = data;

    this->run_count++;
}

JudgeLightData* JudgeLightCycle::GetData(int cnt) {
    JudgeLightData* cur = this->datas;
    int cur_cnt = 0;
    while (cur) {
        if (cur_cnt++ == cnt) {
            return cur;
        }
        cur = cur->next;
    }
    return nullptr;
}
