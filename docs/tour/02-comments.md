---
title: 02. Comments
---

Sabre supports only two types of comments. Although inline comments are ignored internally, documentation comments however are preserved within the language-server context as developer information. When running an Sabre program, all comments are discarded as they are found to improve startup performance.

```sabre
// Inline comments in Sabre start with "//" and terminate in an end-of-line.

//! Documentation comments can be marked by a documentation slash "//!". These
//! comments are tokenized as preambles for whatever immediately follows.
```

There are no multi-line comments in Sabre. This allows for non-contextual tokenization of comments.
