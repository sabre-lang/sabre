#ifndef _SABRE_HPP
#define _SABRE_HPP

/// Async Includes
#include "sabre/async/_inline/async.ipp"

/// Bundle Includes
#include "sabre/bundle/archive.hpp"
#include "sabre/bundle/codec.hpp"
#include "sabre/bundle/fuse.hpp"
#include "sabre/bundle/options.hpp"
#include "sabre/bundle/service.hpp"
#include "sabre/bundle/worker.hpp"

/// Builtin Includes
#include "sabre/builtins/adapter.hpp"
#include "sabre/builtins/inspect.hpp"
#include "sabre/builtins/storage.hpp"
#include "sabre/builtins/wrapper.hpp"

/// Bytecode Includes
#include "sabre/bytecode/allocator.hpp"
#include "sabre/bytecode/assembler.hpp"
#include "sabre/bytecode/binder.hpp"
#include "sabre/bytecode/block.hpp"
#include "sabre/bytecode/compiler.hpp"
#include "sabre/bytecode/disposable.hpp"
#include "sabre/bytecode/glyph.hpp"
#include "sabre/bytecode/instruction.hpp"
#include "sabre/bytecode/invoker.hpp"
#include "sabre/bytecode/iterator.hpp"
#include "sabre/bytecode/label.hpp"
#include "sabre/bytecode/loop.hpp"
#include "sabre/bytecode/metadata.hpp"
#include "sabre/bytecode/operands.hpp"
#include "sabre/bytecode/optimizer.hpp"
#include "sabre/bytecode/request.hpp"
#include "sabre/bytecode/routine.hpp"
#include "sabre/bytecode/trace.hpp"
#include "sabre/bytecode/visitor.hpp"

/// Crate Includes
#include "sabre/crate/constants.hpp"
#include "sabre/crate/manifest.hpp"
#include "sabre/crate/registry.hpp"

/// Diagnostic Includes
#include "sabre/diagnostic/inspect.hpp"
#include "sabre/diagnostic/options.hpp"
#include "sabre/diagnostic/reporter.hpp"
#include "sabre/diagnostic/scope.hpp"
#include "sabre/diagnostic/view.hpp"

/// Document Includes
#include "sabre/document/buffer.hpp"
#include "sabre/document/publisher.hpp"
#include "sabre/document/service.hpp"

/// Engine Includes
#include "sabre/engine/dispatch.hpp"
#include "sabre/engine/exports.hpp"
#include "sabre/engine/frame.hpp"
#include "sabre/engine/interrupt.hpp"
#include "sabre/engine/invoke.hpp"
#include "sabre/engine/metadata.hpp"
#include "sabre/engine/operand.hpp"

/// Flow Includes
#include "sabre/flow/control.hpp"
#include "sabre/flow/effect.hpp"
#include "sabre/flow/passable.hpp"
#include "sabre/flow/unreachable.hpp"

/// Format Includes
#include "sabre/format/collection.hpp"
#include "sabre/format/comments.hpp"
#include "sabre/format/delimited.hpp"
#include "sabre/format/dispatch.hpp"
#include "sabre/format/mixin.hpp"
#include "sabre/format/options.hpp"
#include "sabre/format/pipeline.hpp"
#include "sabre/format/reader.hpp"
#include "sabre/format/storage.hpp"
#include "sabre/format/wrap.hpp"
#include "sabre/format/writer.hpp"

/// Garbage Includes
#include "sabre/garbage/lifetimes.hpp"
#include "sabre/garbage/marker.hpp"
#include "sabre/garbage/mode.hpp"
#include "sabre/garbage/options.hpp"
#include "sabre/garbage/service.hpp"

/// Globals Includes
#include "sabre/globals/roots.hpp"
#include "sabre/globals/service.hpp"

/// Handle Includes
#include "sabre/handle/local.hpp"
#include "sabre/handle/scope.hpp"
#include "sabre/handle/stack.hpp"

/// Heap Includes
#include "sabre/heap/buffer.hpp"
#include "sabre/heap/limits.hpp"
#include "sabre/heap/options.hpp"
#include "sabre/heap/region.hpp"
#include "sabre/heap/service.hpp"
#include "sabre/heap/storage.hpp"

/// Image Includes
#include "sabre/image/arena.hpp"
#include "sabre/image/binary.hpp"
#include "sabre/image/records.hpp"
#include "sabre/image/slice.hpp"

/// Import Includes
#include "sabre/import/graph.hpp"
#include "sabre/import/loader.hpp"
#include "sabre/import/service.hpp"
#include "sabre/import/storage.hpp"
#include "sabre/import/worker.hpp"

/// Lexer Includes
#include "sabre/lexer/buffer.hpp"
#include "sabre/lexer/dispatch.hpp"
#include "sabre/lexer/inspect.hpp"
#include "sabre/lexer/kind.hpp"
#include "sabre/lexer/options.hpp"
#include "sabre/lexer/scanner.hpp"
#include "sabre/lexer/token.hpp"
#include "sabre/lexer/visitor.hpp"

/// Lifecycle Includes
#include "sabre/lifecycle/scope.hpp"
#include "sabre/lifecycle/service.hpp"

/// Locale Includes
#include "sabre/locale/generator.hpp"
#include "sabre/locale/options.hpp"
#include "sabre/locale/service.hpp"

/// Machine Includes
#include "sabre/machine/allocator.hpp"
#include "sabre/machine/builder.hpp"
#include "sabre/machine/constants.hpp"
#include "sabre/machine/emitter.hpp"
#include "sabre/machine/facts.hpp"
#include "sabre/machine/frame.hpp"
#include "sabre/machine/info.hpp"
#include "sabre/machine/logger.hpp"
#include "sabre/machine/service.hpp"
#include "sabre/machine/visitor.hpp"

/// Member Includes
#include "sabre/member/descriptor.hpp"
#include "sabre/member/factory.hpp"
#include "sabre/member/property.hpp"
#include "sabre/member/reference.hpp"
#include "sabre/member/storage.hpp"

/// Module Includes
#include "sabre/module/abstract.hpp"
#include "sabre/module/archive.hpp"
#include "sabre/module/dynamic.hpp"
#include "sabre/module/metadata.hpp"
#include "sabre/module/phase.hpp"
#include "sabre/module/script.hpp"

/// Operator Includes
#include "sabre/operator/attribute.hpp"
#include "sabre/operator/inspect.hpp"
#include "sabre/operator/kind.hpp"
#include "sabre/operator/storage.hpp"

/// Parser Includes
#include "sabre/parser/delimited.hpp"
#include "sabre/parser/dispatch.hpp"
#include "sabre/parser/snapshot.hpp"
#include "sabre/parser/stream.hpp"

/// Product Includes
#include "sabre/product/details.hpp"
#include "sabre/product/semver.hpp"

/// Reflect Includes
#include "sabre/reflect/category.hpp"
#include "sabre/reflect/inspect.hpp"

/// Relint Includes
#include "sabre/relint/context.hpp"
#include "sabre/relint/exports.hpp"
#include "sabre/relint/hooks.hpp"
#include "sabre/relint/mirror.hpp"
#include "sabre/relint/options.hpp"
#include "sabre/relint/rule.hpp"
#include "sabre/relint/scope.hpp"
#include "sabre/relint/service.hpp"

/// Register Includes
#include "sabre/register/scoped.hpp"
#include "sabre/register/slot.hpp"
#include "sabre/register/span.hpp"

/// Resource Includes
#include "sabre/resource/frame.hpp"
#include "sabre/resource/location.hpp"
#include "sabre/resource/scheme.hpp"
#include "sabre/resource/trace.hpp"

/// Runtime Includes
#include "sabre/runtime/allocator.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/runtime/entry.hpp"
#include "sabre/runtime/executor.hpp"
#include "sabre/runtime/isolate.hpp"
#include "sabre/runtime/options.hpp"

/// Server Includes
#include "sabre/server/connection.hpp"
#include "sabre/server/events.hpp"
#include "sabre/server/options.hpp"
#include "sabre/server/style.hpp"
#include "sabre/server/transport.hpp"
#include "sabre/server/utilities.hpp"
#include "sabre/server/worker.hpp"

/// Shape Includes
#include "sabre/shape/limits.hpp"
#include "sabre/shape/lookup.hpp"
#include "sabre/shape/service.hpp"

/// Signal Includes
#include "sabre/signal/service.hpp"

/// Syntax Includes
#include "sabre/syntax/bounds.hpp"
#include "sabre/syntax/comparator.hpp"
#include "sabre/syntax/grammar.hpp"
#include "sabre/syntax/metadata.hpp"
#include "sabre/syntax/node.hpp"
#include "sabre/syntax/precedence.hpp"
#include "sabre/syntax/tree.hpp"
#include "sabre/syntax/trivia.hpp"
#include "sabre/syntax/visitor.hpp"

/// Testing Includes
#include "sabre/testing/entry.hpp"
#include "sabre/testing/inspect.hpp"
#include "sabre/testing/options.hpp"
#include "sabre/testing/service.hpp"
#include "sabre/testing/storage.hpp"

/// Toolchain Includes
#include "sabre/toolchain/bundle.hpp"
#include "sabre/toolchain/format.hpp"
#include "sabre/toolchain/launch.hpp"
#include "sabre/toolchain/parse.hpp"
#include "sabre/toolchain/serve.hpp"
#include "sabre/toolchain/test.hpp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Value Includes
#include "sabre/value/_inline/value.ipp"

/// Variable Includes
#include "sabre/variable/analyzer.hpp"
#include "sabre/variable/captures.hpp"
#include "sabre/variable/modifiers.hpp"
#include "sabre/variable/scope.hpp"
#include "sabre/variable/upvalues.hpp"
#include "sabre/variable/visitor.hpp"

#endif
