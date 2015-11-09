#!/usr/bin/env python3

from scope_harness import (
    CategoryListMatcher, CategoryListMatcherMode, CategoryMatcher,
    Parameters, ResultMatcher, ScopeHarness
)
from scope_harness.testing import *
import unittest, sys, os
from subprocess import Popen, PIPE

class AppsTest (ScopeHarnessTestCase):
    @classmethod
    def setUpClass(cls):
        cls.process = Popen(["/usr/bin/python3", FAKE_SERVER], stdout=PIPE)
        port = cls.process.stdout.readline().decode("utf-8").rstrip('\n')
        os.environ["NETWORK_SCOPE_APIROOT"] = "localhost"
        os.environ["NETWORK_SCOPE_APIPORT"] = port

    @classmethod
    def tearDownClass(cls):
        cls.process.terminate()


    def start_harness(self):
        self.harness = ScopeHarness.new_from_scope_list(Parameters([SCOPE_INI]))
        self.view = self.harness.results_view
        self.view.active_scope = SCOPE_NAME


    def test_surfacing_results(self):
        self.start_harness()
        self.view.search_query = ''

        match = CategoryListMatcher() \
            .has_exactly(2) \
            .mode(CategoryListMatcherMode.BY_ID) \
            .category(CategoryMatcher("current") \
                .title("London, GB") \
                .has_at_least(1) \
                .result(ResultMatcher("2643743") \
                    .title("21.83°C") \
                    .art("http://openweathermap.org/img/w/02d.png") \
                    .subtitle("few clouds") ) ) \
            .category(CategoryMatcher("forecast") \
                .title("7 day forecast") \
                .has_at_least(7) \
                .result(ResultMatcher("500") \
                    .title("18.84°C to 25.09°C") \
                    .art("http://openweathermap.org/img/w/10d.png") \
                    .subtitle("light rain") ) \
                .result(ResultMatcher("501") \
                    .title("15.54°C to 20.86°C") \
                    .art("http://openweathermap.org/img/w/10d.png") \
                    .subtitle("moderate rain") ) \
                .result(ResultMatcher("802") \
                    .title("13.19°C to 19.58°C") \
                    .art("http://openweathermap.org/img/w/03d.png") \
                    .subtitle("scattered clouds") ) \
                .result(ResultMatcher("501") \
                    .title("13.46°C to 18.09°C") \
                    .art("http://openweathermap.org/img/w/10d.png") \
                    .subtitle("moderate rain") ) \
                .result(ResultMatcher("502") \
                    .title("15.39°C to 17.38°C") \
                    .art("http://openweathermap.org/img/w/10d.png") \
                    .subtitle("heavy intensity rain") ) \
                .result(ResultMatcher("501") \
                    .title("16.17°C to 18.44°C") \
                    .art("http://openweathermap.org/img/w/10d.png") \
                    .subtitle("moderate rain") ) \
                .result(ResultMatcher("500") \
                    .title("16.67°C to 19.02°C") \
                    .art("http://openweathermap.org/img/w/10d.png") \
                    .subtitle("light rain") ) ) \
            .match(self.view.categories)
        self.assertMatchResult(match)


    def test_search_results(self):
        self.start_harness()
        self.view.search_query = 'Manchester,uk'

        match = CategoryListMatcher() \
            .has_exactly(2) \
            .mode(CategoryListMatcherMode.BY_ID) \
            .category(CategoryMatcher("current") \
                .title("Manchester, GB") \
                .has_at_least(1) \
                .result(ResultMatcher("2643123") \
                    .title("17.35°C") \
                    .art("http://openweathermap.org/img/w/03d.png") \
                    .subtitle("scattered clouds") ) ) \
            .category(CategoryMatcher("forecast") \
                .title("7 day forecast") \
                .has_at_least(7) \
                .result(ResultMatcher("800") \
                    .title("11.96°C to 18.83°C") \
                    .art("http://openweathermap.org/img/w/01d.png") \
                    .subtitle("sky is clear") ) \
                .result(ResultMatcher("501") \
                    .title("12.33°C to 18.59°C") \
                    .art("http://openweathermap.org/img/w/10d.png") \
                    .subtitle("moderate rain") ) \
                .result(ResultMatcher("803") \
                    .title("10.79°C to 17.11°C") \
                    .art("http://openweathermap.org/img/w/04d.png") \
                    .subtitle("broken clouds") ) \
                .result(ResultMatcher("501") \
                    .title("12.21°C to 15.98°C") \
                    .art("http://openweathermap.org/img/w/10d.png") \
                    .subtitle("moderate rain") ) \
                .result(ResultMatcher("500") \
                    .title("12.99°C to 15.73°C") \
                    .art("http://openweathermap.org/img/w/10d.png") \
                    .subtitle("light rain") ) \
                .result(ResultMatcher("501") \
                    .title("14.08°C to 17.64°C") \
                    .art("http://openweathermap.org/img/w/10d.png") \
                    .subtitle("moderate rain") ) \
                .result(ResultMatcher("501") \
                    .title("13.37°C to 15.52°C") \
                    .art("http://openweathermap.org/img/w/10d.png") \
                    .subtitle("moderate rain") ) ) \
            .match(self.view.categories)
        self.assertMatchResult(match)


if __name__ == '__main__':
    SCOPE_NAME = sys.argv[1]
    SCOPE_INI = sys.argv[2]
    FAKE_SERVER = sys.argv[3]

    unittest.main(argv = sys.argv[:1])
