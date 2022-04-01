module.exports = {
    "env": {
        "commonjs": true,
        "es6": true,
        "node": true,
        "browser": false
    },
    "extends": [
        "eslint:recommended",
    ],
    "parser": "@typescript-eslint/parser",
    "globals": {
        "Atomics": "readonly",
        "SharedArrayBuffer": "readonly"
    },
    "parserOptions": {
        "sourceType": "module",
        "ecmaVersion": 12,
        "allowImportExportEverywhere": true,
        "codeFrame": true
    },
    "rules": {
    },
    "plugins": [
        "@typescript-eslint"
    ]
};
