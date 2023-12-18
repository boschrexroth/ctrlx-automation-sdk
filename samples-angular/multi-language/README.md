# README multi-language.angular

This multi-language sample is an Angular application to demonstrate the use of localization files in ctrlX apps.

## Introduction

We use the _NGX-Translate_ library to localize our webapps.
Details of this library can be found here: http://www.ngx-translate.com/

The basic concept of __localization__ is to extract __localizable texts__ from HTML and Type Script and to put them into language specific localization files.
At runtime localized texts are loaded from localization files depending on the currently selected language and displayes within the UI.

## Localization Files

### Concept and Notation
All texts to translate are stored in json __localization files__.
Localization files are loaded at runtime from: __https://<ip>/assets/i18n/__

+ To ensure that several localization files can be located in the assets directory in parallel, they have to comply to the following naming convention:
    __webAppName.(lang-ISO_639-1-code).json__
+ Localization files contain localizable texts for exactly one language and for exactly one webapp.
+ For ISO codes refer to https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes

  ```bash
  Example:
  myApp.en.json
  myApp.de.json

#### Content and structure

Localization files are basically _key/values_ lists. Each _key/value_ pair represents one localizable string.

The basic format of a key/value entry is:

    "<key>": "<translated text>"

### Key naming convention

Keys have to comply the following convention to avoid conflicts when several localization files are loaded simultaneously at runtime:

    (webApp id).(component).(element)[.type]

+ _webapp id_: to make keys unique across all webapps and id has to be part of a key (e.g. "plc", "wrk", "frw" ,"idm"...)
+ _component_: denotes the building block (angular component, service etc.) within a webapp where the translatable text is located (e.g. login-dialog,control-details,common-texts...)
+ _element_: denotes the ui-element within a component on which the text is placed (e.g. connectionfailed, userimage, acceleration...)
+ _type_: denotes the type of the ui-element. This gives translators additional information that is relevant for translation. It is only required for the certain ui-element types

  ```bash
  title  (page / dialog titles)
  header (column headers etc.)
  tab (tab page titles)
  menu (sidebar menus and other menus)
  sub menu (sidebar menus and other menus)
  button (button text)
  tooltip (tooltip text max length?)

### Localization file formats
There are two formats of localization files: the flat format and the namespaced format.

_Flat format_

In flat format each key/value pair corresponds to one line within the json file.

The flat format is the straight forward approach that can be used especially in small webapps that do not have much translatable text.
This is quite simple. Though with increasing file length it becomes difficult to keep overview about the file content.

    Example
    {
      "frw.configStartPage.packetFlowOverview": "Packet flow overview",
      "frw.configStartPage.configurePacketFiltering": "Configure packet filtering",
      "frw.configStartPage.configureNat": "Configure network address translation"
    }

or

    {
      "wrk.settings.title": "Setting",
      "wrk.settings.help.header": "Help",
      "wrk.settings.control-emulation.header": "Virtual control emulation",
      "wrk.settings.control-emulation.tooltip": "Only evaluated when starting a control",
      "wrk.settings.control-emulation": "Show the emulation process window for a running ctrlX COREvirtual"
      }

_Namespaced format_

To add structure to extensive localization files the namespaced format can be used.

In this format the effective key (e.g. "frw.configStartPage.packetFlowOverview") will be composed of the namespace (e.g. "frw.configStartPage) and the key within the namespace (e.g. "packetFlowOverview").
This means that both formats are equivalent and represent the same localization information.

    Example:
    {
      "frw.configStartPage": {
          "packetFlowOverview": "Packet flow overview",
          "configurePacketFiltering": "Configure packet filtering",
          "configureNat": "Configure network address translation"
        }
    }

or

    {
      "wrk.settings": {
      "title": "Setting",
      "help.header": "Help",
      "control-emulation.header": "Virtual control emulation",
      "control-emulation.tooltip": "Only evaluated when starting a control",
      "control-emulation": "Show the emulation process window for a running ctrlX COREvirtual"
    }

_IMPORTANT_

Only add texts to be translated in the file. Texts that should not be translated do not belong in the translation file.

It is not possible to add comments into to a translation file.


## Responsibilites of webapps regarding localization

WebApps
+ are responsible to comply naming conventions for localization files
+ are responsible to transfer the localization files of their used weblibs to their own assets folder (angular.json => glob)
+ are responsible to load their own localization files as well as the localization files of their used weblibs (app.module.ts => _TranslationModule_, _MultiTranslateHttpLoader_)
+ are responsible to set the current language as well as the fallback language

## __How to make webapps localizable__

### __Install ngx-translate and ngx-translate-multi-http-loader__
Use the following commands to install ngx-translate and ngx-translate-multi-http-loader:

    npm install @ngx-translate/core @ngx-translate/http-loader rxjs --save
    npm install ngx-translate-multi-http-loader --save

### __Add an assets/i18n folder to your webapp project__
Add the assets-folder within the src-folder in webapps, as listed below:

    |- <webapp-name>
        |- src              <= sources are here
            |- assets
            |   |- i18n    <= localization files are located here
            |       |- <webapp-name>.en.json
            |       |- <webapp-name>.de.json|
        |- app ...

### __Add the assets folder of weblibs used in the webapp to the assets array in angular.json__

    {
     "$schema": "./node_modules/@angular/cli/lib/config/schema.json",
     ...
     "projects": {
       "webapp.<app-name>": {
        ...
          "architect": {
            "build": {
              "builder": "@angular-devkit/build-angular:browser",
              "options": {
                "outputPath": "dist/<app-name>",
                "index": "src/index.html",
                "main": "src/main.ts",
                "polyfills": "src/polyfills.ts",
                "tsConfig": "tsconfig.app.json",
                "baseHref": "/webapp.<app-name>/",
                ...
                "assets": [
                  "src/assets"
                ],
                styles": [
                  "src/styles.scss"
                ],
                "scripts": []
              },
              ...
    }

### __Add the TranslateModule to app.module.ts__

    import { TranslateModule, TranslateLoader } from '@ngx-translate/core';
    import { MultiTranslateHttpLoader } from 'ngx-translate-multi-http-loader';

    export function createMultiTranslateHttpLoader(http: HttpClient): TranslateLoader {
      return new MultiTranslateHttpLoader(http, [

    //This is how it goes for apps that uses lazy loaded modules (example: firewall app)
    { prefix: './<webapp-name>/assets/i18n/<webapp-name>.', suffix: '.json' },   <= add this for the webapp
    { prefix: './<webapp-name>/assets/i18n/<weblib-name.', suffix: '.json' },    <= add such a line for each weblib used in the webapp

    //This is how it goes for apps that does NOT use lazy loaded modules (example: webapp.motion)
    { prefix: './assets/i18n/<webapp-name>.', suffix: '.json' },                 <= add this for the webapp
    { prefix: './assets/i18n/<weblib-name.', suffix: '.json' },                  <= add such a line for each weblib used in the webapp
    ]);
    }

    @NgModule({
      declarations: [
        AppComponent
      ],
      imports: [
        ...,
        TranslateModule.forRoot({
          loader: {
            provide: TranslateLoader,
            useFactory: createMultiTranslateHttpLoader,
            deps: [HttpClient]
          },
          defaultLanguage: 'en'
       }),
       ...

_NOTE_

Note that in webapp modules TranslateModule.forRoot() must be called.
Note that English (en) is defined as defaultLanguage. That means English is the fallback language if a translation is not available in the currently selected language.

### Add the TranslateModule to lazy-loaded feature modules in <feature-name>.module.ts

    import { HttpClientModule} from '@angular/common/http';
    import { TranslateModule } from '@ngx-translate/core';

    @NgModule({
      declarations: [
        ...
      ],
      imports: [
        ...,
        HttpClientModule,
        TranslateModule.forChild({
          extend: true
        }),
        ...
      ],

_NOTE_

Note that in feature modules TranslateModule.forChild() must be called.
### Localize the UI related parts of your webapp
Please see details here: https://github.com/ngx-translate/core

### Add the TranslateModule for unit tests
Because of injected translate service it is necessary to modify unit tests of app.component (app.component.spec.ts) and other components (<other>.component.spec.ts).

It is also very important to make the change in the "app-routing.module.spec.ts" as well.

    import { TranslateModule} from '@ngx-translate/core';

    describe('AppComponent', () => {
      ...
      beforeEach(waitForAsync(() => {
       TestBed.configureTestingModule({
          imports: [
            ...,
            TranslateModule.forRoot(),   // This is sufficcient. Note that only the text keys are displayed when unit tests are executed.
           ...
         ],
          ...
        })
        .compileComponents();
      }));

## Translation process

The json language file can be translated using common translation tools like Passolo or BabelEdit.

## 7. Enable locale specific formatting with the angular pipe operators

### Problem
I need to format a long decimal number with thousand separators (123456789 → 123.456.789, assuming de-DE formatting). <br>
When I use the Angular DecimalPipe I get "123,456,789" as a result regardless of the language settings (DE or EN) in the ctrlX app.<br>

I tried to supply a locale to the DecimalPipe like this:

      <td mat-cell *matCellDef="let item">{{ item.sum | number:'0.0-0':'de-DE'}}</td>

The idea here is to provide a locale derived from the value of currentLang of the TranslateService.

I get the error:

_ERROR Error: NG02100: InvalidPipeArgument: 'Missing locale data for the locale "de-DE".' for pipe 'DecimalPipe'._

The reason for this error seems to be a missing registration for the locale de-DE<br>
It seems that at the moment there is no general guideline on how to handle locale aware formatting within our application.
### Expected behavior
When the user changes between languages in the ctrlX app, the formatting of numbers and dates is influcenced by that change.<br>
As a developer I can use the standard Angular pipes like DecimalPipe and the formatting of the output respects the current language selection.<br>
This must be managed globally. This is not a responsibility of a single feature module.<br>
As there are multiple locales per language (e.g. en-US, en-GB) , we must either decide on a fixed locale per language or we must provide the user with the ability to select the locale in addition to the language.
### Solution
After some research here is a possible solution. It involves the following steps.

+ Register a provider that will a provide a value of the LOCALE_ID that is based on the currently selected language.
+ Register the localeData for the German language.

Here are the changes necessary to make this work. Everything is located in the app.module file.

    import { APP_INITIALIZER, LOCALE_ID, NgModule} from '@angular/core';
    import { registerLocaleData } from "@angular/common";
    import localeDe from "@angular/common/locales/de";

    @NgModule({
      declarations: [
      // existing code omitted
      ],
      imports: [
      // existing code omitted
      ],
      providers: [
        {
        // existing code omitted
        }, {
            provide: LOCALE_ID,
            deps: [TranslateService],
            useFactory: (translateService: TranslateService) => translateService.currentLang
           }
      ],
      bootstrap: [AppComponent]
    })

    export class AppModule {
      constructor() {
        registerLocaleData(localeDe);
      }
     }




## Gratulations - We're finished - Let's start coding!

## Support
### Developer Community

Please join the [Developer Community](https://developer.community.boschrexroth.com/)

### SDK Forum

Please visit the [SDK Forum](https://developer.community.boschrexroth.com/t5/ctrlX-AUTOMATION/ct-p/dcdev_community-bunit-dcae/)

### Issues

If you've found an error in these sample, please [file an issue](https://github.com/boschrexroth)

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT
