# CMake generated Testfile for 
# Source directory: /home/louis/DOT_Indev/3rd_party/libnabo-master/tests
# Build directory: /home/louis/DOT_Indev/3rd_party/libnabo-master/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
ADD_TEST(validation-2D-exhaustive "/knnvalidate" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.2d.txt" "10" "-1")
ADD_TEST(validation-2D-random "/knnvalidate" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.2d.txt" "10" "10000")
ADD_TEST(validation-3D-exhaustive "/knnvalidate" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.txt" "10" "-1")
ADD_TEST(validation-3D-random "/knnvalidate" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.txt" "10" "10000")
ADD_TEST(validation-3D-large-random "/knnvalidate" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.large.txt" "10" "1000")
ADD_TEST(validation-3D-large-random-radius "/knnvalidate" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.large.txt" "10" "1000" "0.5")
ADD_TEST(bench-3D-large-exhaustive-10000-K1 "/knnbench" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.large.txt" "1" "-10000" "3" "5")
ADD_TEST(bench-3D-large-exhaustive-1000-K1 "/knnbench" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.large.txt" "1" "-1000" "3" "5")
ADD_TEST(bench-3D-large-exhaustive-100-K1 "/knnbench" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.large.txt" "1" "-100" "3" "5")
ADD_TEST(bench-3D-large-random-K1 "/knnbench" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.large.txt" "1" "40000" "3" "5")
ADD_TEST(bench-3D-large-exhaustive-10000-K10 "/knnbench" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.large.txt" "10" "-10000" "3" "5")
ADD_TEST(bench-3D-large-exhaustive-1000-K10 "/knnbench" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.large.txt" "10" "-1000" "3" "5")
ADD_TEST(bench-3D-large-exhaustive-100-K10 "/knnbench" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.large.txt" "10" "-100" "3" "5")
ADD_TEST(bench-3D-large-random-K20 "/knnbench" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.large.txt" "10" "40000" "3" "5")
ADD_TEST(bench-3D-large-exhaustive-10000-K30 "/knnbench" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.large.txt" "30" "-10000" "3" "5")
ADD_TEST(bench-3D-large-exhaustive-1000-K30 "/knnbench" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.large.txt" "30" "-1000" "3" "5")
ADD_TEST(bench-3D-large-exhaustive-100-K30 "/knnbench" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.large.txt" "30" "-100" "3" "5")
ADD_TEST(bench-3D-large-random-K30 "/knnbench" "/home/louis/DOT_Indev/3rd_party/libnabo-master/tests/data/scan.3d.large.txt" "30" "40000" "3" "5")
