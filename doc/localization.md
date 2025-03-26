This document is part of the Bosch Rexroth ctrlX OS App Development Guideline, and gives hints on how to support user interface localization on ctrlX OS.

Please refer to the App Development Guide for further information.

### **Table of Contents**

[1 Introduction](#introduction)

[2 Localization Files](#files)

[3 Best practice: How to make an app localizable](#practice)

# 1 Introduction <a name="introduction"></a>

ctrlX OS uses the NGX-Translate library to localize Apps

Details of this library can be found here:
<https://ngx-translate.org/>

The basic concept of localization is to

- extract localizable texts from HTML and Typescript
- and put them into language specific localization files.

At runtime, localized texts are loaded from localization files depending on the currently selected language and displayed in the UI.

# 2 Localization Files <a name="files"></a>

## 2.1 Conventions

The texts to be translated are stored in json localization files.
Localization files are loaded at runtime from

```code
https://{ip-address}/assets/i18n/
```

To ensure that several localization files can be located in the assets directory in parallel, they have to comply to the following naming convention:

```code
{App-Name}.{lang-ISO_639-1-code}.json
```

Localization files contain localizable texts for exactly one language and for exactly one app
That means: Every app contains separate localization files per language.

For ISO codes refer to
<https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes>

Examples:

- ctrlx-app.en.json
- ctrlx-app.de.json

## 2.2 Content and structure

Localization files are basically key/values lists. Each key/value pair represents one localizable string.

The basic format of a key/value entry is:

```code
"<key>":  "<translated text>"
```

### 2.2.1 Key naming convention

Keys have to follow the following convention to avoid conflicts when several localization files are loaded simultaneously at runtime:

```code
<app id>.<component>.<element>[.type]
```

- **app id**: unique app id (as defined during the app onboarding process, see App Development Guide for details)
- **component**: denotes the building block (angular component, service etc.) within an app where the translatable text is located
- **element**: denotes the ui-element within a component on which the text is placed
- **type** (optional): denotes the type of the ui-element. This can give  translators additional information,  that may be relevant for translation.

For example, types in ctrlX OS are:

- title  (page / dialog titles)
- header (column headers etc.)
- tab (tab page titles)
- menu (sidebar menus and other menus)
- sub menu (sidebar menus and other menus)
- button (button text)
- tooltip (tooltip text)

### 2.2.2 Localization file formats

Two formats of localization files are supported:

- the flat format
- the namespaced format

Each of them has its pros and cons.

### Flat format

In flat format each key/value pair corresponds to one line within the json file.

The flat format is the straight forward approach that can be used especially in small webapps/weblibs that do not have much translatable text.
This is quite simple. However, with increasing file length it becomes difficult to keep the overview about the file content.

Localization file in flat format:

```json
{
  "myapp.startPage.element1": "Localized Text 1",
  "myapp.startPage.element2": "Localized Text 2",
  "myapp.startPage.element3": "Localized Text 3"
}
```

### Namespaced format

To add structure to extensive localization files the namespaced format can be used.

In this format the effective key (e.g. *"myapp.startPage.element1"*) will be composed of the namespace (e.g. *"myapp.startPage"*) and the key within the namespace (e.g. *"element1"*).
This means that both formats are equivalent and represent the same localization information.

Localization file in namespaced format:

```json
{
  "myapp.startPage": {
    "element1": "Localized Text 1",
    "element2": "Localized Text 2",
    "element3": "Localized Text 3"
  }
}
```

### Note

Comments and remarks are not supported in localization files

# 3 Best practice: How to make an app localizable <a name="practice"></a>

**Note**:

This chapter describes a ctrlX OS best practice to add localization to an app.
Bosch Rexroth does not guarantee that this practice will work for Partner Apps.

Please see <https://ngx-translate.org/> for additional information.
This site also provides example for different options on how to add ngx translate to a web application.

## 3.1 Install ngx-translate and ngx-translate-multi-http-loader

Use the following commands to install ngx-translate and ngx-translate-multi-http-loader:

```bash
    npm install @ngx-translate/core @ngx-translate/http-loader rxjs --save
    npm install ngx-translate-multi-http-loader --save
```

## 3.2 Add an assets/i18n folder to your app project

```code
<root>
  |- projects
      |- <app-name>
           |- src         <= sources are here
               |
               |- assets
               |   |- i18n    <= localization files are located here
               |       |- <app-name>.en.json
               |       |- <app-name>.xy.json
               |
               |- ...
```

## 3.3 Add the TranslateModule to app.module.ts

```code
import { TranslateModule, TranslateLoader } from '@ngx-translate/core';
import { MultiTranslateHttpLoader } from 'ngx-translate-multi-http-loader';

export function createMultiTranslateHttpLoader(http: HttpClient): TranslateLoader {
  return new MultiTranslateHttpLoader(http, [

    //If the app uses lazy loaded modules:
    { prefix: './<app-name>/assets/i18n/<app-name>.', suffix: '.json' },

    //If the app does NOT use lazy loaded modules
    { prefix: './assets/i18n/<app-name>.', suffix: '.json' },

  ]);
}

@NgModule({
  declarations: [
    AppComponent
  ],
  imports: [
    ...,
    HttpClientModule,
    TranslateModule.forRoot({
      loader: {
          provide: TranslateLoader,
          useFactory: createMultiTranslateHttpLoader,
          deps: [HttpClient]
      },
      defaultLanguage: 'en'
    }),
    ...
```

Note that TranslateModule.forRoot() must be called.

For ctrlX OS, English (en) is defined as defaultLanguage. That means English is the fallback language if a translation is not available in the currently selected language.

## 3.4 Localize the UI related parts of your app

Please see details here: <https://github.com/ngx-translate/core>

****
**Copyright**
© Bosch Rexroth AG 2022-2024

This document, as well as the data, specifications and other information set forth in it, are the exclusive property of Bosch
