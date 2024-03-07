import sys


def attach_to_debugger(host, port):
    try:
        # TODO : Use your PyCharm install directory.
        pydev_path = "C:\\Users\\fcors\\AppData\\Local\\Programs\\PyCharm Professional\\plugins\\python\\helpers\\pydev"

        if pydev_path not in sys.path:
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
        print("PyCharm Remote Debug enabled on %s:%s." % (host, port))
    except Exception:
        import traceback
        traceback.print_exc()


if __name__ == "__main__":
    attach_to_debugger('localhost', 60058)
