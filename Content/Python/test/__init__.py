import os
import sys

PROJECT_PATH = os.getcwd()
SOURCE_PATH = os.path.join(PROJECT_PATH, "src")
TEST_PATH = os.path.join(PROJECT_PATH, "test")

sys.path.append(SOURCE_PATH)
sys.path.append(TEST_PATH)
