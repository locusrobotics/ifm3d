#include <chrono>
#include <cstdint>
#include <memory>
#include <thread>
#include "gtest/gtest.h"
#include <ifm3d/deserialize.h>
#include <ifm3d/fg/buffer.h>
#include <ifm3d/device/o3r.h>
#include <ifm3d/device/err.h>
#include <ifm3d/fg.h>
#include <ifm3d/deserialize/struct_tof_info_v3.hpp>
#include <ifm3d/deserialize/struct_tof_info_v4.hpp>
#include <ifm3d/deserialize/struct_rgb_info_v1.hpp>
#include <ifm3d/deserialize/struct_o3r_ods_info_v1.hpp>
#include <ifm3d/deserialize/struct_o3r_ods_occupancy_grid_v1.hpp>
#include <algorithm>
#include <fstream>
#include "tof_info_test_data.hpp"
#include "rgb_info_test_data.hpp"
#include "test_utils.hpp"
#include <limits>

TEST(DeserializeTestWithFile, struct_tof_info_v3_size_exception)
{
  auto buffer = ifm3d::Buffer(1, 200, 1, ifm3d::pixel_format::FORMAT_8U);

  EXPECT_THROW(ifm3d::TofInfoV3::Deserialize(buffer), ifm3d::Error);
}
TEST(DeserializeTestWithFile, struct_tof_info_v3)
{
  auto buffer = ifm3d::read_buffer_from_file("tof_info.data");
  auto tof_info_v3 = ifm3d::TofInfoV3::Deserialize(buffer);
  constexpr auto minimum_required_version = 3;
  EXPECT_GE(tof_info_v3.version, minimum_required_version);
  EXPECT_NEAR(tof_info_v3.amplitude_resolution,
              ifm3d::tof_info::amplitude_resolution,
              ifm3d::epsilon);
  EXPECT_NEAR(tof_info_v3.distance_resolution,
              ifm3d::tof_info::distance_resolution,
              ifm3d::epsilon);
  EXPECT_TRUE(
    ifm3d::compare_array(tof_info_v3.amp_normalization_factors,
                         ifm3d::tof_info::amp_normalization_factors));

  std::array<float, 6> extrinc_opt_to_user = {
    tof_info_v3.extrisic_optic_to_user.transX,
    tof_info_v3.extrisic_optic_to_user.transY,
    tof_info_v3.extrisic_optic_to_user.transZ,
    tof_info_v3.extrisic_optic_to_user.rotX,
    tof_info_v3.extrisic_optic_to_user.rotY,
    tof_info_v3.extrisic_optic_to_user.rotZ};

  EXPECT_TRUE(ifm3d::compare_array(extrinc_opt_to_user,
                                   ifm3d::tof_info::extrincsic_optic_to_user));

  EXPECT_EQ(tof_info_v3.intrinsic_calibration.modelID,
            ifm3d::tof_info::intrinsic_calib_model_id);

  EXPECT_TRUE(
    ifm3d::compare_array(tof_info_v3.intrinsic_calibration.modelParameters,
                         ifm3d::tof_info::intrinsic_calib_model_param));

  EXPECT_EQ(tof_info_v3.inverse_intrinsic_calibration.modelID,
            ifm3d::tof_info::inverse_intrinsic_calib_model_id);

  EXPECT_TRUE(ifm3d::compare_array(
    tof_info_v3.inverse_intrinsic_calibration.modelParameters,
    ifm3d::tof_info::inverse_intrinsic_calib_model_param));

  EXPECT_TRUE(ifm3d::compare_array(tof_info_v3.exposure_timestamps_ns,
                                   ifm3d::tof_info::exposure_timestamps_ns));

  EXPECT_TRUE(ifm3d::compare_array(tof_info_v3.exposure_times_s,
                                   ifm3d::tof_info::exposure_times_s));

  EXPECT_NEAR(tof_info_v3.illu_temperature,
              ifm3d::tof_info::illu_temperature,
              ifm3d::epsilon);

  EXPECT_TRUE(ifm3d::compare_array(tof_info_v3.mode, ifm3d::tof_info::mode));

  EXPECT_TRUE(
    ifm3d::compare_array(tof_info_v3.imager, ifm3d::tof_info::imager));
}

TEST(DeserializeTestWithFile, struct_tof_info_v4_size_exception)
{
  auto buffer = ifm3d::Buffer(1, 200, 1, ifm3d::pixel_format::FORMAT_8U);

  EXPECT_THROW(ifm3d::TofInfoV4::Deserialize(buffer), ifm3d::Error);
}
TEST(DeserializeTestWithFile, struct_tof_info_v4)
{

  auto buffer = ifm3d::read_buffer_from_file("tof_info.data");

  auto tof_info_v4 = ifm3d::TofInfoV4::Deserialize(buffer);
  constexpr auto minimum_required_version = 4;
  EXPECT_GE(tof_info_v4.version, minimum_required_version);
  EXPECT_NEAR(tof_info_v4.amplitude_resolution,
              ifm3d::tof_info::amplitude_resolution,
              ifm3d::epsilon);
  EXPECT_NEAR(tof_info_v4.distance_resolution,
              ifm3d::tof_info::distance_resolution,
              ifm3d::epsilon);
  EXPECT_TRUE(
    ifm3d::compare_array(tof_info_v4.amp_normalization_factors,
                         ifm3d::tof_info::amp_normalization_factors));

  std::array<float, 6> extrinc_opt_to_user = {
    tof_info_v4.extrisic_optic_to_user.transX,
    tof_info_v4.extrisic_optic_to_user.transY,
    tof_info_v4.extrisic_optic_to_user.transZ,
    tof_info_v4.extrisic_optic_to_user.rotX,
    tof_info_v4.extrisic_optic_to_user.rotY,
    tof_info_v4.extrisic_optic_to_user.rotZ};

  EXPECT_TRUE(ifm3d::compare_array(extrinc_opt_to_user,
                                   ifm3d::tof_info::extrincsic_optic_to_user));

  EXPECT_EQ(tof_info_v4.intrinsic_calibration.modelID,
            ifm3d::tof_info::intrinsic_calib_model_id);

  EXPECT_TRUE(
    ifm3d::compare_array(tof_info_v4.intrinsic_calibration.modelParameters,
                         ifm3d::tof_info::intrinsic_calib_model_param));

  EXPECT_EQ(tof_info_v4.inverse_intrinsic_calibration.modelID,
            ifm3d::tof_info::inverse_intrinsic_calib_model_id);

  EXPECT_TRUE(ifm3d::compare_array(
    tof_info_v4.inverse_intrinsic_calibration.modelParameters,
    ifm3d::tof_info::inverse_intrinsic_calib_model_param));

  EXPECT_TRUE(ifm3d::compare_array(tof_info_v4.exposure_timestamps_ns,
                                   ifm3d::tof_info::exposure_timestamps_ns));

  EXPECT_TRUE(ifm3d::compare_array(tof_info_v4.exposure_times_s,
                                   ifm3d::tof_info::exposure_times_s));

  EXPECT_NEAR(tof_info_v4.illu_temperature,
              ifm3d::tof_info::illu_temperature,
              ifm3d::epsilon);

  EXPECT_TRUE(ifm3d::compare_array(tof_info_v4.mode, ifm3d::tof_info::mode));

  EXPECT_TRUE(
    ifm3d::compare_array(tof_info_v4.imager, ifm3d::tof_info::imager));

  EXPECT_EQ(tof_info_v4.measurement_block_index,
            ifm3d::tof_info::measurement_block_index);
  EXPECT_NEAR(tof_info_v4.measurement_range_min,
              ifm3d::tof_info::measurement_range_min,
              ifm3d::epsilon);
  EXPECT_NEAR(tof_info_v4.measurement_range_max,
              ifm3d::tof_info::measurement_range_max,
              ifm3d::epsilon);
}

TEST(DeserializeTestWithFile, struct_rgb_info_v1_size_exception)
{
  auto buffer = ifm3d::Buffer(1, 200, 1, ifm3d::pixel_format::FORMAT_8U);

  EXPECT_THROW(ifm3d::RGBInfoV1::Deserialize(buffer), ifm3d::Error);
}
TEST(DeserializeTestWithFile, struct_rgb_info_v1)
{
 auto buffer = ifm3d::read_buffer_from_file("rgb_info.data");
  auto rgb_info_v1 = ifm3d::RGBInfoV1::Deserialize(buffer);

  constexpr auto minimum_required_version = 1;
  EXPECT_GE(rgb_info_v1.version, minimum_required_version);
  EXPECT_EQ(rgb_info_v1.frame_counter, ifm3d::rgb_info::frame_counter);
  EXPECT_EQ(rgb_info_v1.timestamp_ns, ifm3d::rgb_info::timestamp_ns);
  EXPECT_NEAR(rgb_info_v1.exposure_time,
              ifm3d::rgb_info::exposure_time,
              ifm3d::epsilon);

  std::array<float, 6> extrinc_opt_to_user = {
    rgb_info_v1.extrisic_optic_to_user.transX,
    rgb_info_v1.extrisic_optic_to_user.transY,
    rgb_info_v1.extrisic_optic_to_user.transZ,
    rgb_info_v1.extrisic_optic_to_user.rotX,
    rgb_info_v1.extrisic_optic_to_user.rotY,
    rgb_info_v1.extrisic_optic_to_user.rotZ};

  EXPECT_TRUE(ifm3d::compare_array(extrinc_opt_to_user,
                                   ifm3d::rgb_info::extrincsic_optic_to_user));

  EXPECT_EQ(rgb_info_v1.intrinsic_calibration.modelID,
            ifm3d::rgb_info::intrinsic_calib_model_id);

  EXPECT_TRUE(
    ifm3d::compare_array(rgb_info_v1.intrinsic_calibration.modelParameters,
                         ifm3d::rgb_info::intrinsic_calib_model_param));

  EXPECT_EQ(rgb_info_v1.inverse_intrinsic_calibration.modelID,
            ifm3d::rgb_info::inverse_intrinsic_calib_model_id);

  EXPECT_TRUE(ifm3d::compare_array(
    rgb_info_v1.inverse_intrinsic_calibration.modelParameters,
    ifm3d::rgb_info::inverse_intrinsic_calib_model_param));
}

TEST(DeserializeTestWithFile, struct_ods_info_v1_size_exception)
{
  auto buffer = ifm3d::Buffer(1, 5, 1, ifm3d::pixel_format::FORMAT_8U);

  EXPECT_THROW(ifm3d::ODSInfoV1::Deserialize(buffer), ifm3d::Error);
}

TEST(DeserializeTestWithFile, struct_ods_info_v1)
{
  // TODO : when device will be avaliable
}

TEST(DeserializeTestWithFile, struct_ods_occupancy_grid_v1_size_exception)
{
  auto buffer = ifm3d::Buffer(1, 5, 1, ifm3d::pixel_format::FORMAT_8U);

  EXPECT_THROW(ifm3d::ODSOccupancyGridV1::Deserialize(buffer), ifm3d::Error);
}

TEST(DeserializeTestWithFile, struct_ods_occupancy_grid_info_v1)
{
  // TODO : when device will be avaliable
}
