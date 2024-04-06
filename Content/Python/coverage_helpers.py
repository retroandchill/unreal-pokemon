import os
import sys
from logging import error
from typing import Type, AnyStr
from unittest import TestResult, TestLoader, TextTestRunner, TestCase

from coverage import Coverage
from unreal import Paths

cov = Coverage(include=[os.path.join(Paths.project_dir(), "Content/Python/src/*")],
                    omit=[os.path.join(Paths.engine_dir(),
                                      "Plugins/Experimental/PythonScriptPlugin/Content/Python/*"),
                         os.path.join(Paths.project_dir(), "Content/Python/test/*")])
cov.start()


def run_test_with_coverage(test_class: Type[TestCase], test_file_name: os.PathLike[AnyStr]) -> TestResult:
    test_file = os.path.join(Paths.project_dir(), "coverage-reports",
                             f"{os.path.splitext(os.path.basename(test_file_name))[0]}.xml")

    suite = TestLoader().loadTestsFromTestCase(test_class)
    result = TextTestRunner(stream=sys.stdout, buffer=True).run(suite)
    cov.stop()
    cov.save()
    cov.xml_report(outfile=test_file)
    cov.start()

    if not result.wasSuccessful():
        error('Test run failed!')

    return result
