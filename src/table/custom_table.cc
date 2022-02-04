#include "custom_table.h"
#include <cstring>
#include <math.h>

namespace bytedance_db_project {
    CustomTable::CustomTable() : index_tree_(4) {}

    CustomTable::~CustomTable() {
        delete rows_;
    }

    void CustomTable::Load(BaseDataLoader *loader) {
        // TODO: Implement this!

        num_cols_ = loader->GetNumCols();
        auto rows = loader->GetRows();
        num_rows_ = rows.size();
        char pcol0[FIXED_FIELD_LEN];
        rows_ = new char[FIXED_FIELD_LEN * num_rows_ * num_cols_];

        for (auto row_id = 0; row_id < num_rows_; row_id++) {
            auto cur_row = rows.at(row_id);
            std::memcpy(rows_ + row_id * (FIXED_FIELD_LEN * num_cols_), cur_row,
                        FIXED_FIELD_LEN * num_cols_);
            std::memcpy(pcol0, cur_row, FIXED_FIELD_LEN);
            index_tree_.insert(DECODE_INT32(pcol0), cur_row);
        }


        // AnalyzeTable
        btree_iterator it = btree_iterator(&index_tree_);
        int32_t n_unique = 0;
        int32_t max_val_len = 0;
        while (it.hasNext()) {
            auto kv = it.next();
            n_unique += 1;
            max_val_len = max(max_val_len, (int32_t) kv.second.size());
        }

        if (n_unique * log10(num_rows_) / num_rows_ < (num_rows_ * num_cols_) / (n_unique * 2 * 4 * max_val_len)) {
            /*  isWorthIndex */
        } else {
            index_tree_.clear();
            // not worth for index
        }

    }

    int32_t CustomTable::GetIntField(int32_t row_id, int32_t col_id) {
        // TODO: Implement this!
        try {
            if (!rows_) {
                // can't load when tab not exist!
                throw std::runtime_error("null exception");
            }
            // i * M +j,M=num_cols
            int32_t offset = FIXED_FIELD_LEN * ((row_id * num_cols_) + col_id);

            memset(val_addr,0,sizeof(val_addr));

            WRITE_BYTE(val_addr, rows_ + offset, FIXED_FIELD_LEN);

            // recast to int32_t pointer for convert from byte array
            int32_t val = DECODE_INT32(val_addr);

            // for other check in future
            return val;
        } catch (std::exception &e) {
            e.what();
            exit(1);
        }
    }

    void CustomTable::PutIntField(int32_t row_id, int32_t col_id, int32_t field) {
        // TODO: Implement this!
        try {
            // safe check
            if (!rows_) {
                throw std::runtime_error("null exception");
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
            int32_t offset = FIXED_FIELD_LEN * ((row_id * num_cols_) + col_id);
            memset(val_addr,0,sizeof(val_addr));
            memcpy(val_addr,rows_+offset,FIXED_FIELD_LEN);
            int32_t old_field;
            if((old_field=DECODE_INT32(val_addr)) == field) return;

            WRITE_BYTE(rows_ + offset, ENCODE_INT32(field), FIXED_FIELD_LEN);

            // if col_id == 0

        } catch (std::exception &e) {
            e.what();
            exit(1);
        }
    }

    int64_t CustomTable::ColumnSum() {
        // TODO: Implement this!
        int64_t result = 0;
        std::pair<int32_t, vector<val_t>> kv;
        if (!index_tree_.empty()) {
            btree_iterator btree_iter = btree_iterator(&index_tree_);
            while (btree_iter.hasNext()) {
                kv = btree_iter.next();
                result += (int64_t) (kv.first * kv.second.size());
            }
        } else {
            // not hit btree cached index
            for (int i = 0; i < num_rows_; ++i) {
                result += GetIntField(i, 0);
            }
        }
        return result;
    }

    int64_t CustomTable::PredicatedColumnSum(int32_t threshold1,
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

    int64_t CustomTable::PredicatedAllColumnsSum(int32_t threshold) {
        // TODO: Implement this!
        int64_t result = 0;

        std::pair<int32_t, vector<val_t>> kv;
        memset(val_addr,0,sizeof(val_addr));

        if (!index_tree_.empty()) {
//            auto it = btree_iterator(&index_tree_);
            auto it = btree_iterator(&index_tree_,threshold);
            while (it.hasNext()) {
                kv = it.next();
                if (kv.first > threshold) {
                    // calculate
                    for (auto &row_memory: kv.second) {
                        for (int j = 1; j < num_cols_; ++j) {
                            std::memcpy(val_addr, row_memory + j * FIXED_FIELD_LEN, FIXED_FIELD_LEN);
                            result += DECODE_INT32(val_addr);
                        }
                    }
                    result += kv.first * kv.second.size();
                }
            }
        } else {
            for (int i = 0; i < num_rows_; ++i) {
                if (GetIntField(i, 0) > threshold) {
                    for (int j = 0; j < num_cols_; ++j) {
                        result += GetIntField(i, j);
                    }
                }
            }
        }
        return result;
    }

    int64_t CustomTable::PredicatedUpdate(int32_t threshold) {
        // TODO: Implement this!
        // update and return count
        int64_t count = 0;
        int32_t newValue = 0;
        char pcol2[FIXED_FIELD_LEN];
        char pcol3[FIXED_FIELD_LEN];
        if (!index_tree_.empty()) {
            auto it = btree_iterator(&index_tree_);
            while (it.hasNext()) {
                auto kv = it.next();
                if (kv.first < threshold) {
                    for (auto &row_memory: kv.second) {

                        std::memcpy(pcol2, row_memory + FIXED_FIELD_LEN * 2, FIXED_FIELD_LEN);
                        std::memcpy(pcol3, row_memory + FIXED_FIELD_LEN * 3, FIXED_FIELD_LEN);
                        newValue = DECODE_INT32(pcol3) + DECODE_INT32(pcol2);
                        std::memcpy(row_memory + 3 * FIXED_FIELD_LEN, ENCODE_INT32(newValue), FIXED_FIELD_LEN);
                    }
                    count += kv.second.size();

                } else {
                    break;
                }
            }
        } else {
            int32_t tmp = 0;
            for (int i = 0; i < num_rows_; ++i) {
                if ((tmp = GetIntField(i, 0)) < threshold) {
                    count += 1;
                    if ((tmp = GetIntField(i, 2)) != 0) {
                        PutIntField(i, 3, GetIntField(i, 3) + tmp);
                    }
                }
            }
        }
        return count;
    }
} // namespace bytedance_db_project