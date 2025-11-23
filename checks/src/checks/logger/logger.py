"""Module initializing logger for mstd checks."""
import logging

logging.basicConfig(level=logging.INFO)

cpp_check_logger = logging.getLogger("cpp_checks")
