#include "column_table.h"
#include "concurrent_task.h"
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

namespace bytedance_db_project {
    ColumnTable::ColumnTable() {}

    ColumnTable::~ColumnTable() {
        if (columns_ != nullptr) {
            delete columns_;
            columns_ = nullptr;
        }
    }

//
// columnTable, which stores data in column-major format.
// That is, data is laid out like
//   col 1 | col 2 | ... | col m.
//
    void ColumnTable::Load(BaseDataLoader *loader) {
        num_cols_ = loader->GetNumCols();
        std::vector<char *> rows = loader->GetRows();
        num_rows_ = rows.size();
        columns_ = new char[FIXED_FIELD_LEN * num_rows_ * num_cols_]; /*META * ROWS * COLS */

        for (int32_t row_id = 0; row_id < num_rows_; row_id++) {
            auto cur_row = rows.at(row_id);
            for (int32_t col_id = 0; col_id < num_cols_; col_id++) {
                int32_t offset = FIXED_FIELD_LEN * ((col_id * num_rows_) + row_id);
                std::memcpy(columns_ + offset, cur_row + FIXED_FIELD_LEN * col_id,
                            FIXED_FIELD_LEN);
            }
        }
    }

    int32_t ColumnTable::GetIntField(int32_t row_id, int32_t col_id) {
        // TODO: Implement this!
        try {
            if (!columns_) {
                // can't load when tab not exist!
                throw std::runtime_error("null exception");
            }
            memset(val_addr, 0, sizeof(val_addr));


            int32_t offset = FIXED_FIELD_LEN * ((col_id * num_rows_) + row_id);
            WRITE_BYTE(val_addr, columns_ + offset, FIXED_FIELD_LEN);

            // recast to int32_t pointer for convert from byte array
            return DECODE_INT32(val_addr);

        } catch (std::exception &e) {
            e.what();
            exit(1);
        }
    }

    void ColumnTable::PutIntField(int32_t row_id, int32_t col_id, int32_t field) {
        // TODO: Implement this!
        try {
            // safe check
            if (!columns_) {
                throw std::runtime_error("null exception\n");
            }
            try {
                if (field > FIXED_FIELD_MAX) {
                    throw std::overflow_error("field value not allowed\n");
                } else if (field < FIXED_FIELD_MIN) {
                    throw std::underflow_error("field value not allowed\n");
                }
            } catch (std::overflow_error &e) {
                // cut field to accepted MAX
                e.what();
                field = FIXED_FIELD_MAX;
            } catch (std::underflow_error &e) {
                // cut field to accepted MIN
                e.what();
                field = FIXED_FIELD_MIN;
            }

            int32_t offset = FIXED_FIELD_LEN * ((col_id * num_rows_) + row_id);
            memset(val_addr, 0, sizeof(val_addr));
            memcpy(val_addr, columns_ + offset, FIXED_FIELD_LEN);
            if (field == DECODE_INT32(val_addr)) return;

            WRITE_BYTE(columns_ + offset, ENCODE_INT32(field), FIXED_FIELD_LEN);

        } catch (std::exception &e) {
            e.what();
            return;
        }
    }

    int64_t ColumnTable::ColumnSum() {
        // TODO: Implement this!

        if(sysconf(_SC_NPROCESSORS_ONLN)>8){
            pthread_mutexattr_t mutexattr;
            pthread_mutexattr_init(&mutexattr);
            pthread_mutex_init(&lock, &mutexattr);
            count = 0; // init count

            int32_t n_task = 4;
            pthread_t threads[n_task];
            pthread_attr_t threads_attr[n_task];
            std::pair<ColumnTable *, std::pair<int, int>> args[n_task];
            int32_t prev = 0;
            int32_t step = int32_t((num_rows_+1) / n_task);
            int32_t next = std::min(prev + step, num_rows_);

            for (int i = 0; i < n_task; ++i) {
                pthread_attr_init(&threads_attr[i]);
                args[i] = {this, {prev, next}};
                pthread_create(&threads[i], &threads_attr[i], range_calc, (void *) &args[i]);

                prev = next;
                next = std::min(prev + step, num_rows_);
            }

            for (int i = 0; i < n_task; ++i) {
                pthread_join(threads[i], nullptr);
            }
            return count;

        } else{
            int64_t result = 0;

            for (int i = 0; i < num_rows_; ++i) {
                result += GetIntField(i, 0);
            }
            return result;
        }

    }

    int64_t ColumnTable::PredicatedColumnSum(int32_t threshold1,
                                             int32_t threshold2) {
        // TODO: Implement this!
        int64_t result = 0;
        for (int i = 0; i < num_rows_; ++i) {
            if (GetIntField(i, 1) > threshold1 && GetIntField(i, 2) < threshold2) {
                result += GetIntField(i, 0);
            }
        }
        return result;
    }

    int64_t ColumnTable::PredicatedAllColumnsSum(int32_t threshold) {
        // TODO: Implement this!
        int64_t result = 0;

        for (int i = 0; i < num_rows_; ++i) {
            if (GetIntField(i, 0) > threshold) {
                for (int j = 0; j < num_cols_; ++j) {
                    result += GetIntField(i, j);
                }
            }
        }
        return result;
    }

    int64_t ColumnTable::PredicatedUpdate(int32_t threshold) {
        // TODO: Implement this!
        int64_t count = 0;
        int32_t tmp = 0;
        int32_t offset = 0;
        auto calcOffset = [](const int32_t& num_rows,const int32_t& row_id,const int32_t& col_id) {
            return FIXED_FIELD_LEN * ((col_id * num_rows) + row_id);

        };

        memset(val_addr, 0, sizeof(val_addr));
        for (int i = 0; i < num_rows_; ++i) {
            if ((GetIntField(i, 0)) < threshold) {
                count += 1;
                if ((tmp = GetIntField(i, 2)) != 0) {
                    offset = calcOffset(num_rows_, i, 3);

                    std::memcpy(val_addr, columns_ + offset, FIXED_FIELD_LEN);
                    int32_t Value = DECODE_INT32(val_addr);
                    Value += tmp;
                    WRITE_BYTE(columns_ + offset, ENCODE_INT32(Value), FIXED_FIELD_LEN);

                    //                    PutIntField(i, 3, GetIntField(i, 3) + tmp);
                }
            }
        }
        return count;
    }
} // namespace bytedance_db_project