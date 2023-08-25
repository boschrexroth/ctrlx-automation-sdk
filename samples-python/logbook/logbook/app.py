# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import logging
from cysystemd import journal

def run():
    """run
    """
    print("Simple snap in Python using logging with different log levels", flush=True)
        
    log = logging.getLogger()
    log.setLevel(logging.DEBUG)
    log.addHandler(journal.JournaldLogHandler())

    log.exception("I am an exception message")
    log.critical("I am a critical message")
    log.error("I am an error")
    log.warning("I am a warning")
    log.info("I am an info message")
    log.debug("I am a debug message")
    
    #<timestamp>|<userId>|<mainDiagnosisCode>|<mainTitle>|<detailedDiagnosisCode>|<detailedTitle>|<entity>|<dynamicSource>|<dynamicDescription>
 