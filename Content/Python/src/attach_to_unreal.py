import sys

def attach_to_debugger(host, port):
    try:
        pydev_path = "C:/Users/fcors/AppData/Local/Programs/PyCharm Professional/plugins/python/helpers/pydev"

        if not pydev_path in sys.path:
            sys.path.append(pydev_path)
        import pydevd
        pydevd.stoptrace()
        pydevd.settrace(
            port=port,
            host=host,
            stdoutToServer=True,
            stderrToServer=True,
            overwrite_prev_trace=True,
            suspend=False,
            trace_only_current_thread=False,
            patch_multiprocessing=False,
        )
        print("PyCharm Remote Debug enabled on %s:%s." % (host,port))
    except:
        import traceback
        traceback.print_exc()

attach_to_debugger('localhost', 60058)