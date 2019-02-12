#import signal
import sys
import os
import time

from .mts_agent_python import *
from .mts_import import *

setGlobals(globals())

Strategy=genMtsStratgyClass()
