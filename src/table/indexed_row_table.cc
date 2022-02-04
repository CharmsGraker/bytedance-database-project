#include "indexed_row_table.h"
#include<cstring>
#include <numeric>

/** IndexedRowTable can be useful for RowTable
 * to avoid load all rows, create index on some column */


namespace bytedance_db_project {
    IndexedRowTable::IndexedRowTable(int32_t index_column) {
        index_column_ = index_column;
    }

    void IndexedRowTable::Load(BaseDataLoader *loader) {
        // TODO: Implement this!
        // create index on index column

        char pcol0[FIXED_FIELD_LEN];
        if (index_.empty()) {
            // trigger to fullLoad Table
            num_cols_ = loader->GetNumCols();
            auto rows = loader->GetRows();
            num_rows_ = rows.size();
            rows_ = new char[FIXED_FIELD_LEN * num_rows_ * num_cols_];
            for (auto row_id = 0; row_id < num_rows_; row_id++) {
                auto cur_row = rows.at(row_id);
                std::memcpy(rows_ + row_id * (FIXED_FIELD_LEN * num_cols_), cur_row,
                            FIXED_FIELD_LEN * num_cols_);
                std::memcpy(pcol0, cur_row, FIXED_FIELD_LEN);
                index_[DECODE_INT32(pcol0)].push_back(row_id);
            }

        } else {

        };
        // load Index
//        loadIndexOnColumn(index_column_);
    }

    int32_t IndexedRowTable::GetIntField(int32_t row_id, int32_t col_id) {
        // TODO: Implement this!
        try {
            if (!rows_) {
                // can't load when tab not exist!
                throw std::runtime_error("null exception");
            }
            // i * M +j,M=num_cols
            int32_t offset = FIXED_FIELD_LEN * ((row_id * num_cols_) + col_id);

            memset(val_addr, 0, sizeof(val_addr));
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

    void IndexedRowTable::PutIntField(int32_t row_id, int32_t col_id,
                                      int32_t field) {
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
            memset(val_addr, 0, sizeof(val_addr));
            memcpy(val_addr, rows_ + offset, FIXED_FIELD_LEN);

            // have not modified, return
            if (DECODE_INT32(val_addr) == field) return;

            if(col_id==0) {
                int32_t tmp = DECODE_INT32(val_addr);
                std::vector<int32_t>::iterator it = std::lower_bound(index_[tmp].begin(),index_[tmp].end(),row_id);
                index_[tmp].erase(it);

                it = std::lower_bound(index_[field].begin(),index_[field].end(),row_id);
                index_[field].insert(it,row_id);
            }

            WRITE_BYTE(rows_ + offset, ENCODE_INT32(field), FIXED_FIELD_LEN);
        } catch (std::exception &e) {
            e.what();
            return;
        }
    }

    int64_t IndexedRowTable::ColumnSum() {
        // TODO: Implement this!
        int64_t result = 0;

        if (index_.empty()) {
            for (int i = 0; i < num_rows_; ++i) {
                result += GetIntField(i, 0);
            }
        } else {
            for (auto &cache: index_) {
                result += cache.first * cache.second.size();
            }
        }
        return result;
    }

    int64_t IndexedRowTable::PredicatedColumnSum(int32_t threshold1,
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

    int64_t IndexedRowTable::PredicatedAllColumnsSum(int32_t threshold) {
        // TODO: Implement this!
        int64_t result = 0;
        if (index_.empty()) {
            for (int i = 0; i < num_rows_; ++i) {
                if (GetIntField(i, 0) > threshold) {
                    for (int j = 0; j < num_cols_; ++j) {
                        result += GetIntField(i, j);
                    }
                }
            }
        } else {
            for (auto &cache: index_) {
                if (cache.first > threshold) {
                    for (auto &i: cache.second) {
                        for (int j = 1; j < num_cols_; ++j) {
                            result += GetIntField(i, j);
                        }
                    }
                    result += cache.first * cache.second.size();
                }
            }
        }

        return result;
    }

    int64_t IndexedRowTable::PredicatedUpdate(int32_t threshold) {
        // TODO: Implement this!
        int64_t count = 0;
        if (index_.empty()) {
            for (int i = 0; i < num_rows_; ++i) {
                count += (GetIntField(i, 0) < threshold);
            }
        } else {
            for (auto &cache: index_) {
                if (cache.first < threshold) {
                    count += cache.second.size();
                }
            }
        }
        return count;
    }
} // namespace bytedance_db_project
