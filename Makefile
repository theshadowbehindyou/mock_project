# # Đường dẫn đến thư mục chứa mã nguồn và thư mục chứa tệp tiêu đề
# SRC_DIR = src
# OBJ_DIR = build
# INCLUDE_DIR = include

# # Danh sách các tệp nguồn và tệp đối tượng
# SRC = $(SRC_DIR)/main.cpp $(SRC_DIR)/MediaFile.cpp $(SRC_DIR)/Playlist.cpp $(SRC_DIR)/MediaLibrary.cpp
# OBJ = $(OBJ_DIR)/main.o $(OBJ_DIR)/MediaFile.o $(OBJ_DIR)/Playlist.o $(OBJ_DIR)/MediaLibrary.o

# # Cấu hình trình biên dịch và cờ biên dịch
# CXX = g++
# CXXFLAGS = -std=c++11 -Wall -I$(INCLUDE_DIR)
# LDFLAGS = -lSDL2 -lSDL2_mixer -ltag

# # Quy tắc tạo tệp thực thi
# media_app: $(OBJ)
# 	$(CXX) -o media_app $(OBJ) $(LDFLAGS)

# # Quy tắc tạo các tệp đối tượng từ các tệp nguồn
# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Quy tắc làm sạch
# clean:
# 	rm -rf $(OBJ_DIR)/*.o media_app

# # Quy tắc tạo thư mục build nếu chưa có
# $(OBJ_DIR):
# 	mkdir -p $(OBJ_DIR)

# # Quy tắc mặc định
# all: $(OBJ_DIR) media_app
# Makefile cho dự án media_app

# Trình biên dịch
CC = g++

# Tùy chọn biên dịch
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Thư viện liên kết
LDFLAGS = -lstdc++fs -lSDL2 -ltag

# Thư mục chứa mã nguồn và tệp đầu ra
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Tệp nguồn và tệp đối tượng
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Tên tệp thực thi
EXECUTABLE = media_app

# Quy tắc mặc định
all: $(EXECUTABLE)

# Quy tắc biên dịch tệp thực thi từ các tệp đối tượng
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Quy tắc biên dịch tệp nguồn thành tệp đối tượng
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)  # Tạo thư mục build nếu chưa có
	$(CC) $(CXXFLAGS) -c $< -o $@

# Quy tắc làm sạch
clean:
	rm -f $(BUILD_DIR)/*.o $(EXECUTABLE)

# Quy tắc để in thông tin về các tệp
info:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
