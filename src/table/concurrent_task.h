#ifndef DATABASE_PROJECT_MAIN_CONCURRENT_TASK_H
#define DATABASE_PROJECT_MAIN_CONCURRENT_TASK_H
#include "table/column_table.h"

using namespace bytedance_db_project;
pthread_mutex_t lock;
int64_t count=0;

auto range_calc=[](void* data)-> void * {
    auto& rangeInfo = *(std::pair<ColumnTable*,std::pair<int,int>>*)(data);
    int32_t tmp=0;
    std::pair<int,int>& range = rangeInfo.second;
    ColumnTable* tab = rangeInfo.first;
    for(int i=range.first;i<range.second;++i) {
        tmp += tab->GetIntField(i,0);
    }
    pthread_mutex_lock(&lock);
    count += tmp;
    pthread_mutex_unlock(&lock);
    pthread_exit(nullptr);
};
#endif //DATABASE_PROJECT_MAIN_CONCURRENT_TASK_H
