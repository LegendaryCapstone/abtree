#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include "data.h"
#include "tree.h"

const int worker_num = 5;
const bool test_batch = true;

TEST(DataTest, LoadProto) {

  if(test_batch) {
    const std::string file_name = "BATCH_DATA_FILE";
    DataProvider data_provider = DataProvider(file_name);
    std::cout << "------- PRINT FIRST THREE ROW --------" << std::endl;
    data_provider.print_samples(3);
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;

    std::cout << "------- PRINT GET_BATCH_DATA ---------" << std::endl;
    MatrixPtr batch_data_ptr = std::make_shared<Matrix>();
    data_provider.get_next_batch(batch_data_ptr, 1000);
    batch_data_ptr->Print(3);
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;

    size_t data_height = data_provider.num_samples();
    size_t batch_size = 1000;
    MatrixPtr shuf_batch_data_ptr = std::make_shared<Matrix>();
    for (size_t i = 0; i < (data_height/batch_size*10 + 1); ++i)
    {
      data_provider.get_next_batch(shuf_batch_data_ptr, batch_size);    
    }
    std::cout << "---- PRINT SHUFFLED GET_BATCH_DATA M---" << std::endl;
    shuf_batch_data_ptr->Print(3);
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;


    DataProvider data_provider_s = DataProvider(file_name);
    std::cout << "------- PRINT FIRST THREE ROW --------" << std::endl;
    data_provider_s.print_samples(3);
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;

    std::cout << "------- PRINT GET_BATCH_DATA ---------" << std::endl;
    MatrixPtr batch_data_ptr_ser = std::make_shared<Matrix>();
    data_provider_s.get_next_batch_serial(batch_data_ptr_ser, 1000);
    batch_data_ptr_ser->Print(3);
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;

    MatrixPtr shuf_batch_data_ptr_ser = std::make_shared<Matrix>();
    for (size_t i = 0; i < (data_height/batch_size*10 + 1); ++i)
    {
      data_provider_s.get_next_batch_serial(shuf_batch_data_ptr_ser, batch_size);    
    }
    std::cout << "---- PRINT SHUFFLED GET_BATCH_DATA S---" << std::endl;
    shuf_batch_data_ptr_ser->Print(3);
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;



  } else {
    DataProvider data_providers[worker_num];
    for (size_t i = 0; i < worker_num; ++i) {
      data_providers[i] = DataProvider(std::to_string(i + 1));
    }

    std::cout << "------- PRINT FIRST THREE ROW --------" << std::endl;
    data_providers[0].print_samples(3);
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;

    std::cout << "------- PRINT GET_BATCH_DATA ---------" << std::endl;
    MatrixPtr batch_data_ptr = std::make_shared<Matrix>();
    data_providers[0].get_next_batch(batch_data_ptr, 1000);
    batch_data_ptr->Print(3);
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;

    size_t data_height = data_providers[0].num_samples();
    size_t batch_size = 1000;
    MatrixPtr shuf_batch_data_ptr = std::make_shared<Matrix>();
    for (size_t i = 0; i < (data_height/batch_size + 1); ++i)
    {
      data_providers[0].get_next_batch(shuf_batch_data_ptr, batch_size);    
    }
    std::cout << "---- PRINT SHUFFLED GET_BATCH_DATA ---" << std::endl;
    shuf_batch_data_ptr->Print(3);
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;

    std::cout << "------- PRINT VALIDATION DATA --------" << std::endl;
    MatrixPtr vali_data = data_providers[0].get_validation();
    vali_data->Print(3);
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;

    for (size_t i = 0; i < worker_num; ++i) {
      ASSERT_FLOAT_EQ(data_providers[i].num_feas(), data_providers[(i + 1) % worker_num].num_feas());
    }
    
    for (size_t i = 0; i < worker_num; ++i) {
      ASSERT_FLOAT_EQ(data_providers[i].num_samples(), data_providers[(i + 1) % worker_num].num_samples());
    }

  }
}

// TEST(TreeTest, OneTreePredict) {
//   RegTreePtr tree = std::make_shared<RegTree>();
//   tree->SetType({FeaType::CONT, FeaType::CONT, FeaType::DISC, FeaType::RANK});
//   std::vector<Value> feas = {{.cls=0}, {.cls=1}, {.cls=2}, {.cls=3}};
//   feas.resize(MAX_NODE_SIZE, {.cls=0});
//   std::vector<Value> values = {{.v=0},
//                                {.v=0.5}, {.cls=1}, {.level=1},
//                                {.v=0.4}, {.v=0.5}, {.v=0.6}, {.v=0.7}};
//   values.resize(MAX_NODE_SIZE, {.v=0.0});
//   tree->AddOneTree(feas, values);
// //  tree->Print();

//   MatrixPtr batch = std::make_shared<Matrix>(Matrix(4, 4));
//   batch->SetType({FeaType::CONT, FeaType::CONT, FeaType::DISC, FeaType::RANK});
//   batch->Copy(0, {{.v=0.0}, {.v=1}, {.cls=0}, {.level=2}});
//   batch->Copy(1, {{.v=0.0}, {.v=1}, {.cls=0}, {.level=0}});
//   batch->Copy(2, {{.v=0.0}, {.v=0.4}, {.cls=0}, {.level=0}});
//   batch->Copy(3, {{.v=0.0}, {.v=0.4}, {.cls=1}, {.level=0}});

//   VectorPtr result = std::make_shared<std::vector<float>>();
//   tree->Predict(batch, result);
//   ASSERT_FLOAT_EQ(0.7, (*result)[0]);
//   ASSERT_FLOAT_EQ(0.6, (*result)[1]);
//   ASSERT_FLOAT_EQ(0.75, (*result)[2]);
//   ASSERT_FLOAT_EQ(0.4, (*result)[3]);
// }

// TEST(TreeTest, EnsemblePredict) {
//   RegTreePtr tree = std::make_shared<RegTree>();
//   tree->SetType({FeaType::CONT, FeaType::CONT, FeaType::DISC, FeaType::RANK});
//   std::vector<Value> feas = {{.cls=0}, {.cls=1}, {.cls=2}, {.cls=3}};
//   feas.resize(MAX_NODE_SIZE, {.cls=0});
//   std::vector<Value> values = {{.v=0},
//                                {.v=0.5}, {.cls=1}, {.level=1},
//                                {.v=0.4}, {.v=0.5}, {.v=0.6}, {.v=0.7}};
//   values.resize(MAX_NODE_SIZE, {.v=0.0});
//   tree->AddOneTree(feas, values);
//   tree->AddOneTree(feas, values, 0.5);
// //  tree->Print();


//   MatrixPtr batch = std::make_shared<Matrix>(Matrix(4, 4));
//   batch->SetType({FeaType::CONT, FeaType::CONT, FeaType::DISC, FeaType::RANK});
//   batch->Copy(0, {{.v=0.0}, {.v=1}, {.cls=0}, {.level=2}});
//   batch->Copy(1, {{.v=0.0}, {.v=1}, {.cls=0}, {.level=0}});
//   batch->Copy(2, {{.v=0.0}, {.v=0.4}, {.cls=0}, {.level=0}});
//   batch->Copy(3, {{.v=0.0}, {.v=0.4}, {.cls=1}, {.level=0}});

//   VectorPtr result = std::make_shared<std::vector<float>>();
//   tree->Predict(batch, result);
//   ASSERT_FLOAT_EQ(0.7 + 0.35, (*result)[0]);
//   ASSERT_FLOAT_EQ(0.6 + 0.3, (*result)[1]);
//   ASSERT_FLOAT_EQ(0.5 + 0.25, (*result)[2]);
//   ASSERT_FLOAT_EQ(0.4 + 0.2, (*result)[3]);
// }


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
