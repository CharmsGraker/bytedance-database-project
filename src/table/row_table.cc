#include "row_table.h"
#include <cstring>

namespace bytedance_db_project {
    RowTable::RowTable() {}

    RowTable::~RowTable() {
        if (rows_ != nullptr) {
            delete rows_;
            rows_ = nullptr;
        }
    }

    void RowTable::Load(BaseDataLoader *loader) {
        num_cols_ = loader->GetNumCols();
        auto rows = loader->GetRows();
        num_rows_ = rows.size();
        rows_ = new char[FIXED_FIELD_LEN * num_rows_ * num_cols_];
        for (auto row_id = 0; row_id < num_rows_; row_id++) {
            auto cur_row = rows.at(row_id);
            std::memcpy(rows_ + row_id * (FIXED_FIELD_LEN * num_cols_), cur_row,
                        FIXED_FIELD_LEN * num_cols_);
        }
    }

    int32_t RowTable::GetIntField(int32_t row_id, int32_t col_id) {
        // TODO: Implement this!
        try {
            if (!rows_) {
                // can't load when tab not exist!
                throw std::runtime_error("null exception");
            }
            // i * M +j,M=num_cols
            memset(val_addr,0,sizeof(val_addr));
            int32_t offset = FIXED_FIELD_LEN * ((row_id * num_cols_) + col_id);
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

    void RowTable::PutIntField(int32_t row_id, int32_t col_id, int32_t field) {
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
            if(field == DECODE_INT32(val_addr)) return;
            WRITE_BYTE(rows_ + offset, ENCODE_INT32(field), FIXED_FIELD_LEN);

        } catch (std::exception &e) {
            e.what();
            exit(1);
        }
    }

    int64_t RowTable::ColumnSum() {
        // TODO: Implement this!
        int64_t result = 0;

        for (int i = 0; i < num_rows_; ++i) {
            result += GetIntField(i, 0);
        }
        return result;
    }

    int64_t RowTable::PredicatedColumnSum(int32_t threshold1, int32_t threshold2) {
        // TODO: Implement this!
        int64_t result = 0;
        for (int i = 0; i < num_rows_; ++i) {
            if (GetIntField(i, 1) > threshold1 && GetIntField(i, 2) < threshold2) {
                result += GetIntField(i, 0);
            }
        }
        return result;
    }

    int64_t RowTable::PredicatedAllColumnsSum(int32_t threshold) {
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

    int64_t RowTable::PredicatedUpdate(int32_t threshold) {
        // TODO: Implement this!
        // this method just need to look up threshold for col0?
        int64_t count = 0;
        int32_t tmp=0;
        for (int i = 0; i < num_rows_; ++i) {
            if ((GetIntField(i, 0)) < threshold) {
                count += 1;
                if((tmp= GetIntField(i,2))!=0) {
                    PutIntField(i, 3, GetIntField(i, 3) + tmp);
                }
            }
        }

        return count;
    }
} // namespace bytedance_db_project