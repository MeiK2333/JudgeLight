#include <jl_cycle.h>

void JudgeLightCycle::PushData(JudgeLightData* data) {
    JudgeLightData* head = this->datas;
    while (head->next) {
        head = head->next;
    }
    head->next = data;
}
