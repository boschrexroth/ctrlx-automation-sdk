// Karma configuration file, see link for more information
// https://karma-runner.github.io/1.0/config/configuration-file.html

module.exports = function (config) {
  config.set({
    basePath: '',
    frameworks: ['jasmine', '@angular-devkit/build-angular'],
    plugins: [
      require('karma-jasmine'),
      require('karma-chrome-launcher'),
      require('karma-jasmine-html-reporter'),
      require('karma-coverage'),
      require('@angular-devkit/build-angular/plugins/karma')
    ],
    client: {
      clearContext: false // leave Jasmine Spec Runner output visible in browser
    },
    coverageReporter: {
      dir: require('path').join(__dirname, './coverage'),
      reporters: [
        {type: 'html', subdir: '.'},
        {type: 'lcovonly', subdir: '.'}
      ],
      fixWebpackSourcePaths: true,
      thresholds: {
        statements: 50,
        lines: 50,
        branches: 10,
        functions: 40
      }
    },
    reporters: ['progress', 'kjhtml'],
    port: 9876,
    colors: true,
    logLevel: config.LOG_INFO,
    autoWatch: true,
    browsers: ['Chrome', 'Chrome_NoSandbox', 'ChromeHeadless', 'ChromeHeadless_NoSandbox'],
    singleRun: false,
    restartOnFileChange: true,
    browserDisconnectTolerance: 2,
    browserDisconnectTimeout: 50000,
    browserNoActivityTimeout: 20000,
    customLaunchers: {
      Chrome_NoSandbox: {
        base: 'Chrome',
        flags: ['--no-sandbox']
      },
      ChromeHeadless_NoSandbox: {
          base: 'ChromeHeadless',
          flags: ['--no-sandbox']
      },
      Chrome_with_debugging: {
          base: 'Chrome',
          flags: ['--remote-debugging-port=9222'],
          debug: true
      }
    }
  });
};
