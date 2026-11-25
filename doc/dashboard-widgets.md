## Introduction

ctrlX OS provides a dashboard-like Home screen with widgets, to enable a quick access to information and functionality, without the need to dive into the respective app.

The widgets are provided by the apps. An app can provide an arbitrary number of widgets.

## Technical realization

Widgets are implemented as [Custom Components](https://developer.mozilla.org/docs/Web/Web_Components/Using_custom_elements). This can be done by using the plain web APIs or frameworks like [Stencil](https://stenciljs.com) or [Angular Elements](https://angular.dev/guide/elements).

In short, custom elements are provided in a JavaScript file and define a custom HTML tag. This JavaScript file can be loaded into any web page and the widget is instantiated by just inserting its tag into an HTML file (just like standard tags like <div\> or <table\>). One JavaScript file can contain several Web Components. Your app must serve this JavaScript file, just like other assets, so that it can be addressed via a URL. 

## Widget definition

For the Home screen to know about the widgets, these need to be made publicly known in some way. This is done in the app's manifest in the section ["uiExtensions" → "dashboard"](package-assets.md#dashboard). Each element here defines one widget by providing the information needed to instantiate the widget (like the URL of the JavaScript file, where the widget is implemented, and the widget's tag name, size information) and metadata to display in the widget catalog. For a detailed description see the PackageManager-API, schema "DashboardWidget".

## Configurable widgets

Widgets might need some configuration to work properly (e.g. a weather widget could take a location and a forecast period). This can be achieved by taking the following steps:
1. Define the configuration data structure

   First you need to define the data the widget needs and how it is structured. You can define the data structure freely.

2. Provide initial configuration data to be used when instatiating the widget

   When the widget is added to the dashboard, the user has not done any configuration yet and needs to be provided with an initial configuration using the property ["initialConfig"](package-assets.md#dashboard-widget-initial-config) in the widget's manifest entry. Place the initial configuration in the property ["specificConfig"](package-assets.md#dashboard-widget-specific-config).

3. Make the widget consume the configuration data

   Configuration data is passed to the widget's custom element through the HTML attribute "config" as a JSON string. Your custom element needs to read this attribute and react to value changes.

4. Provide an editor for this configuration data

   A configurable widget must provide its own editor. Like the widget itself, the editor is provided as a custom element and consumes the "config" attribute.
   In order to notify the system of edits made by the user, the editor's custom element must provide a custom event named "configChange". Put the specific config in the event's "detail" property.


The dashboard takes care of displaying the editor, sending the configuration data to the widget and saving it. The widget does not need to care about this.

Here is a minimal example of a configurable widget, just showing a text:

??? example

    ```javascript
    function getTextFromConfig(configString) {
      try {
        return JSON.parse(configString)?.specificConfig?.text;
      } catch (e) {
        console.error("Failed to parse config:", e);
        return "";
      }
    }
    
    class MyWidget extends HTMLElement {
      static observedAttributes = ["config"];
    
      constructor() {
        super();
      }
    
      connectedCallback() {
        this.onConfigChanged(this.getAttribute("config"));
      }
    
      attributeChangedCallback(name, oldValue, newValue) {
        this.onConfigChanged(newValue);
      }
    
      onConfigChanged(configString) {
        this.textContent = getTextFromConfig(configString);
      }
    }
    
    class MyWidgetEditor extends HTMLElement {
      static observedAttributes = ["config"];
    
      constructor() {
        super();
        this.input = null;
      }
    
      connectedCallback() {
        if (!this.input) {
          this.input = document.createElement("input");
          this.input.type = "text";
          this.input.addEventListener("input", () => {
            const specificConfig = { text: this.input.value };
            this.dispatchEvent(new CustomEvent("configChange", { detail: specificConfig }));
          });
          this.appendChild(this.input);
        }
    
        this.input.value = this.getAttribute("config") ?? "";
      }
    
      attributeChangedCallback(name, oldValue, newValue) {
        if (this.input) {
          this.input.value = getTextFromConfig(newValue);
        }
      }
    }
    
    customElements.define("my-widget", MyWidget);
    customElements.define("my-widget-editor", MyWidgetSettings);    
    ```

And this is the corresponding entry in the package manifest:

??? example

    ```json
    {
      ...
      {
        "uiExtensions": {
          "dashboard": [
            {
              "order": 0,
              "src": "/assets/testwidget.js",
              "tag": "my-widget",
              "settingsTag": "my-widget-editor",
              "title": "test widget",
              "categories": [
                "General"
              ],
              "initialConfig": {
                "specificConfig": {
                  "text": "This is my configurable custom widget"
                }
              },
              "initialSize": {
                "cols": 3,
                "rows": 1
              },
              "minSize": {
                "cols": 3,
                "rows": 1
              },
              "maxSize": {
                "cols": 3,
                "rows": 1
              },
              "permissions": []
            }
          ]
        }
      }
      ...
    }

    ```