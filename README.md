# Unreal Metadata Exporter

**Unreal Metadata Exporter** is an Unreal Engine editor plugin that connects Unreal projects to an external, engine-agnostic asset validation pipeline.

The plugin handles **editor integration and asset metadata export only**.  
All validation logic runs outside Unreal Engine.

---

## What It Does

- Integrates with the Unreal Editor
- Exports selected asset **metadata** to JSON

Unreal-specific code is kept minimal and limited to editor concerns.

---

## Pipeline Overview

```
Unreal Editor (metadata export) → JSON → Asset Validator
```

- Unreal Engine: asset selection and metadata extraction  
- Asset Validator: rule evaluation and reporting  

Core validation logic lives here:  
https://github.com/janikowski-dev/Asset-Validator

---

## Design Principles

- Clear separation between export and validation  
- No engine-specific validation rules
- Consistent JSON schema across tools
- Reusable across projects and pipelines  

---

## Installation (for Unreal Developers)

### Requirements

- **Unreal Engine 5.x**
- Project-level plugin installation

---

### Installation Steps

1. **Clone or download the plugin**
   - Clone this repository or download it as a ZIP
   - Extract it if needed

2. **Place the plugin in your project**
   ```
   (YourProject)/Plugins/UnrealMetadataExporter/
   ```

3. **Regenerate project files** (if using C++)
   - Right-click the `.uproject`
   - Select **Generate Visual Studio project files**

4. **Open the project in Unreal Editor**

5. If prompted, **rebuild missing modules**

6. Go to **Edit → Plugins**
   - Locate **Unreal Metadata Exporter**
   - Enable the plugin
   - Restart the editor if requested

---

### Verifying Installation

- Open the **Content Browser**
- Expand the `Tools` menu
- Locate the **Export Assets for Validation** entry provided by the plugin

If the export option is available, the plugin is loaded correctly.

---

### Updating the Plugin

1. Close Unreal Editor
2. Replace the plugin folder with the newer version
3. Regenerate project files if needed
4. Reopen the project and rebuild

---

## Notes for Developers

- This plugin performs **no validation**
- It only extracts editor-visible metadata and serializes it to JSON
- Validation rules live entirely in the external Asset Validator
- Keep engine-facing code:
  - editor-only
  - minimal
  - schema-aligned
