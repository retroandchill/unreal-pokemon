import sys

from . import mock_unreal

sys.modules['unreal'] = mock_unreal
