# TASK: release task receive a params of output file name 
#       and a list of files to be packaged
#       the output file name is the name of the package
#       the list of files is the list of files to be packaged
#       the list of files is a list of files or directories
#       the list of files can be a list of files or directories

export RELEASE_DIR = $(shell pwd)/release

release:
	@echo "release task"
	@echo "output file name: $(OUTPUT_FILE_NAME)"
	@echo "list of files: $(LIST_OF_FILES)"
	@echo "release dir: $(RELEASE_DIR)"
	@echo "release task done"