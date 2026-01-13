⚠ This project is currently in early development.
The API and samples are not yet stable.

# hwpx-owpml-model

## What is this?

`hwpx-owpml-model` is a lightweight OWPML (HWPX) parsing and modeling library for extracting structured content from HWPX documents.

It provides:

* An OOXML-based model for OWPML
* A tree-based representation of document elements (sections, paragraphs, runs, tables, etc.)
* APIs to extract text and document structure for downstream processing (e.g. Markdown, RAG, LLM pipelines)

This project is designed to be used as a **core parsing layer** for:

* HWPX → Markdown converters
* RAG document ingestion pipelines
* Search / indexing systems
* AI-based document understanding

---

## Why this exists

HWPX (OWPML) is a powerful but complex XML-based format.
Directly consuming it in downstream pipelines (RAG, search, summarization, etc.) is difficult without a clean document model.

This project solves that by:

* Converting raw OWPML into a structured object model
* Allowing deterministic traversal of sections and elements
* Making text and layout information accessible for rule-based and AI-based processing

---

## Features

* OOXML-style object model for OWPML
* Section and element traversal
* Text extraction from paragraphs and runs
* Table, block and inline element support
* Sample code for extracting text from a document section

⚠ Current sample code extracts only the **first section** of a document.
This is intentional for demonstration purposes.

---

## Getting started (Windows build)

### Build environment

* Microsoft Windows 10
* Microsoft Visual Studio 2017 (15.9.42)
* Platform: x86

### Build steps

1. Open the solution in Visual Studio
2. Select configuration: `Debug` or `Release`
3. Select platform: `x86`
4. Build the solution

After building, the following files will be generated in `Build/Bin`:

* `Owpml.lib`
* `OWPMLApi.lib`
* `OWPMLUtil.lib`
* `OWPMLTest.exe`

You can link these libraries into your own projects to use the OWPML model.

---

## Running the sample

### From Visual Studio

Set command line arguments in:

```
Project Properties → Debugging → Command Arguments
```

Use:

```
<INPUT_FILE.hwpx> <OUTPUT_FILE.txt>
```

### From command line

```bash
OWPMLTest.exe InputFile.hwpx OutputFile.txt
```

This will extract text from the first section and write it to the output file.

---

## Use in document pipelines

This library is intended to be used as a **low-level parser** in larger systems such as:

* HWPX → Markdown converters
* RAG pipelines (document chunking, embedding, retrieval)
* Search / indexing systems
* AI-based summarization or document understanding

It provides the structured representation required for:

* Rule-based transformations
* AI-assisted table and layout processing

---

## Contribution guidelines

### Code style

* Use spaces instead of tabs
* UTF-8 encoding

### Commit convention

* Separate title and body with a blank line
* Title: within 50 characters
  Format: `<domain>: <summary>`
  Example: `engine: Improve OWPML table traversal`
* Korean: noun phrase
* English: imperative verb form
* Body lines wrap at 72 characters
* Focus on **why** and **how**, not just what

---

## Branch & workflow

### Clone

```bash
git clone https://github.com/hancom-io/hwpx-owpml-model.git
```

### Create a branch

```bash
git checkout -b <your-branch-name>
git push origin <your-branch-name>
```

### Push and create PR

```bash
git status
git add .
git commit -m "#<issue-number> <message>"
git push origin <your-branch-name>
```

Open a Pull Request to merge into `main`.

---

## License

See [LICENSE.txt](LICENSE.txt) for details.

---

## Acknowledgements

This project is based on and inspired by Hancom’s official OWPML reference implementation:
https://github.com/hancom-io/hwpx-owpml-model

It reuses the OWPML object model and XML structure as a foundation, and builds higher-level document processing and AI-oriented pipelines on top of it.

---

## Contact

For questions and discussion, use the GitHub Discussions tab.