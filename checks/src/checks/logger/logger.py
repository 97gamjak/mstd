"""Module initializing logger for mstd checks."""
import logging
import os

__DEBUG_MSTD_CHECKS__ = os.getenv("DEBUG_MSTD_CHECKS", "0")
__DEBUG_MSTD_UTILS__ = os.getenv("DEBUG_MSTD_UTILS", "0")

# TODO(97gamjak): centralize env logic if needed elsewhere
# https://github.com/97gamjak/mstd/issues/26

cpp_check_logger = logging.getLogger("mstd_cpp_checks")

if int(__DEBUG_MSTD_CHECKS__) > 0:
    cpp_check_logger.setLevel(logging.DEBUG)
else:
    cpp_check_logger.setLevel(logging.INFO)

utils_logger = logging.getLogger("mstd_utils")

if int(__DEBUG_MSTD_UTILS__) > 0:
    utils_logger.setLevel(logging.DEBUG)
else:
    utils_logger.setLevel(logging.INFO)
