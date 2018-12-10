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
