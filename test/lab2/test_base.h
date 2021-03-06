#pragma once

#include "data/csv_data_loader.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace bytedance_db_project;
static const std::string kTestCsvFilePath = "./test.csv";
static const int32_t kNumCols = 5;

class TestBase : public ::testing::Test {
protected:
  CsvDataLoader *loader;
  // ensure we always have a fresh registry for tests.
  void SetUp() override {
    loader = new CsvDataLoader(kTestCsvFilePath, kNumCols);
  }

  void TearDown() override { delete loader; }
};
