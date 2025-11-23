"""Module initializing logger for mstd checks."""
import logging
import os

__DEBUG_MSTD_CHECKS__ = os.getenv("DEBUG_MSTD_CHECKS", "0")

# TODO(97gamjak): centralize env logic if needed elsewhere
# https://github.com/97gamjak/mstd/issues/26
if int(__DEBUG_MSTD_CHECKS__) > 0:
    logging.basicConfig(level=logging.DEBUG)
else:
    logging.basicConfig(level=logging.INFO)

cpp_check_logger = logging.getLogger("cpp_checks")
