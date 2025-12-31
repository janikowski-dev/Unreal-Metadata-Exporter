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
- Reusable across projects and pipelines  
- CI-friendly by design  

---

## Scope

This plugin intentionally contains **no validation logic**.

It exists solely to bridge Unreal Engine with an external validation system.
