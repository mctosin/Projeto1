cmake_minimum_required(VERSION 3.15)

project(Projeto1 LANGUAGES C CXX ASM)

find_bsp(ID com.sysprogs.project_importers.stm32.cubemx SOURCE_PROJECT Projeto1.ioc)

add_bsp_based_executable(NAME Projeto1
	GENERATE_BIN
	GENERATE_MAP)

