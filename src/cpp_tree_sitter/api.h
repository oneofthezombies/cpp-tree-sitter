#ifndef CPP_TREE_SITTER_API_H
#define CPP_TREE_SITTER_API_H

#include <memory>
#include <ostream>
#include <string_view>

#include "tree_sitter/api.h"

namespace ts {

using Symbol = TSSymbol;
using SymbolType = TSSymbolType;
using StateId = TSStateId;
using FieldId = TSFieldId;
using LogType = TSLogType;
using InputEncoding = TSInputEncoding;

// CStringDeleter
// --------

class CStringDeleter {
public:
  void operator()(char *c_string_raw) const noexcept;
};

using CStringPtr = std::unique_ptr<char, ts::CStringDeleter>;

// String
// --------

class String {
public:
  explicit String(ts::CStringPtr &&c_string) noexcept;
  String(const ts::String &) = delete;
  String(ts::String &&) noexcept = default;
  ~String() noexcept = default;

  auto operator=(const ts::String &) -> ts::String & = delete;
  auto operator=(ts::String &&) noexcept -> ts::String & = default;

  auto IsNull() const noexcept -> bool;
  auto StringView() const noexcept -> std::string_view;

private:
  ts::CStringPtr c_string_;
};

// Point
// --------

struct Point : public TSPoint {
  explicit Point(const TSPoint &ts_point) noexcept;
  Point(const ts::Point &) noexcept = default;
  Point(ts::Point &&) noexcept = default;
  ~Point() noexcept = default;

  auto operator=(const ts::Point &) noexcept -> ts::Point & = default;
  auto operator=(ts::Point &&) noexcept -> ts::Point & = default;

  auto AsRaw() noexcept -> TSPoint &;
};

auto operator<<(std::ostream &os, const ts::Point &point) -> std::ostream &;

// Node
// --------

class Node {
public:
  explicit Node(TSNode &&ts_node) noexcept;
  Node(const ts::Node &) = default;
  Node(ts::Node &&) noexcept = default;
  ~Node() noexcept = default;

  auto operator=(const ts::Node &) -> ts::Node & = default;
  auto operator=(ts::Node &&) noexcept -> ts::Node & = default;

  auto StartByte() const noexcept -> uint32_t;
  auto StartPoint() const noexcept -> ts::Point;
  auto EndByte() const noexcept -> uint32_t;
  auto EndPoint() const noexcept -> ts::Point;
  auto Symbol() const noexcept -> ts::Symbol;
  auto Type() const noexcept -> std::string_view;
  auto GrammarSymbol() const noexcept -> ts::Symbol;
  auto GrammarType() const noexcept -> std::string_view;
  [[nodiscard]] auto String() const noexcept -> ts::String;
  auto Eq(const ts::Node &other) const noexcept -> bool;
  auto IsNull() const noexcept -> bool;
  auto IsExtra() const noexcept -> bool;
  auto IsNamed() const noexcept -> bool;
  auto IsMissing() const noexcept -> bool;
  auto HasChanges() const noexcept -> bool;
  auto HasError() const noexcept -> bool;
  auto IsError() const noexcept -> bool;
  auto DescendantCount() const noexcept -> uint32_t;
  auto ParseState() const noexcept -> ts::StateId;
  auto NextParseState() const noexcept -> ts::StateId;
  auto Parent() const noexcept -> ts::Node;
  auto Child(const uint32_t index) const noexcept -> ts::Node;
  auto NamedChild(const uint32_t index) const noexcept -> ts::Node;
  auto ChildByFieldId(const ts::FieldId field_id) const noexcept -> ts::Node;
  auto FieldNameForChild(const uint32_t index) const noexcept
      -> std::string_view;
  auto ChildByFieldName(const std::string_view field_name) const noexcept
      -> ts::Node;
  auto ChildCount() const noexcept -> uint32_t;
  auto NamedChildCount() const noexcept -> uint32_t;
  auto NextSibling() const noexcept -> ts::Node;
  auto NextNamedSibling() const noexcept -> ts::Node;
  auto PrevSibling() const noexcept -> ts::Node;
  auto PrevNamedSibling() const noexcept -> ts::Node;
  auto FirstChildForByte(const uint32_t byte) const noexcept -> ts::Node;
  auto FirstNamedChildForByte(const uint32_t byte) const noexcept -> ts::Node;
  auto DescendantForByteRange(const uint32_t start,
                              const uint32_t end) const noexcept -> ts::Node;
  auto NamedDescendantForByteRange(const uint32_t start,
                                   const uint32_t end) const noexcept
      -> ts::Node;
  auto DescendantForPointRange(const ts::Point start,
                               const ts::Point end) const noexcept -> ts::Node;
  auto NamedDescendantForPointRange(const ts::Point start,
                                    const ts::Point end) const noexcept
      -> ts::Node;

  auto AsRaw() noexcept -> TSNode &;

private:
  TSNode ts_node_;
};

auto operator<<(std::ostream &os, const ts::Node &node) -> std::ostream &;

// TSTreeDeleter
// --------

class TSTreeDeleter {
public:
  void operator()(TSTree *ts_tree_raw) const noexcept;
};

using TSTreePtr = std::unique_ptr<TSTree, ts::TSTreeDeleter>;

// Tree
// --------

class Tree {
public:
  explicit Tree(ts::TSTreePtr &&ts_tree) noexcept;
  Tree(const ts::Tree &) = delete;
  Tree(ts::Tree &&) noexcept = default;
  ~Tree() noexcept = default;

  auto operator=(const ts::Tree &) -> ts::Tree & = delete;
  auto operator=(ts::Tree &&) noexcept -> ts::Tree & = default;

  auto RootNode() const noexcept -> ts::Node;
  auto PrintDotGraph(const int file_descriptor) const noexcept -> void;

  auto IsNull() const noexcept -> bool;

  // Consumes the `TSTreePtr` and returns a unique pointer to it.
  [[nodiscard]] auto IntoRaw() noexcept -> ts::TSTreePtr;

  static auto Null() noexcept -> ts::Tree;

private:
  ts::TSTreePtr ts_tree_;
};

auto operator<<(std::ostream &os, const ts::Tree &tree) -> std::ostream &;

// Logger
// --------

class Logger {
public:
  explicit Logger() noexcept;
  Logger(const ts::Logger &) = delete;
  Logger(ts::Logger &&) noexcept = default;
  virtual ~Logger() noexcept = default;

  auto operator=(const ts::Logger &) -> ts::Logger & = delete;
  auto operator=(ts::Logger &&) noexcept -> ts::Logger & = default;

  virtual auto Log(const ts::LogType log_type,
                   const std::string_view buffer) const noexcept -> void = 0;

  auto AsRaw() noexcept -> TSLogger &;

private:
  TSLogger ts_logger_;
};

using LoggerPtr = std::unique_ptr<ts::Logger>;

// ConsoleLogger
// --------

class ConsoleLogger : public ts::Logger {
public:
  explicit ConsoleLogger() noexcept = default;
  ConsoleLogger(const ts::ConsoleLogger &) = delete;
  ConsoleLogger(ts::ConsoleLogger &&) noexcept = default;
  virtual ~ConsoleLogger() noexcept override = default;

  auto operator=(const ts::ConsoleLogger &) -> ts::ConsoleLogger & = delete;
  auto operator=(ts::ConsoleLogger &&) noexcept
      -> ts::ConsoleLogger & = default;

  virtual auto Log(const ts::LogType log_type,
                   const std::string_view buffer) const noexcept
      -> void override;
};

// Language
// --------

class Language {
public:
  explicit Language(const TSLanguage *const ts_language) noexcept;
  Language(const ts::Language &) = delete;
  Language(ts::Language &&) = delete;
  ~Language() noexcept = default;

  auto operator=(const ts::Language &) -> ts::Language & = delete;
  auto operator=(ts::Language &&) -> ts::Language & = delete;

  auto SymbolCount() const noexcept -> uint32_t;
  auto StateCount() const noexcept -> uint32_t;
  auto SymbolName(const ts::Symbol symbol) const noexcept -> std::string_view;

  static constexpr ts::Symbol kSymbolNotFound = 0;

  // If symbol is not found, it returns `kSymbolNotFound`.
  auto SymbolForName(const std::string_view name,
                     const bool is_named) const noexcept -> ts::Symbol;

  auto FieldCount() const noexcept -> uint32_t;
  auto FieldNameForId(const ts::FieldId field_id) const noexcept
      -> std::string_view;
  auto FieldIdForName(const std::string_view name) const noexcept
      -> ts::FieldId;
  auto SymbolType(const ts::Symbol symbol) const noexcept -> ts::SymbolType;
  auto Version() const noexcept -> uint32_t;
  auto NextState(const ts::StateId state,
                 const ts::Symbol symbol) const noexcept -> ts::StateId;

  auto AsRaw() const noexcept -> const TSLanguage *const;

  static auto FromRaw(const TSLanguage *const ts_language) noexcept
      -> ts::Language;

private:
  const TSLanguage *ts_language_;
};

// TSParserDeleter
// --------

class TSParserDeleter {
public:
  // Close the file descriptor before deleting the parser.
  void operator()(TSParser *ts_parser_raw) const noexcept;
};

using TSParserPtr = std::unique_ptr<TSParser, ts::TSParserDeleter>;

using CancellationFlagPtr = std::unique_ptr<size_t>;

// Parser
// --------

class Parser {
public:
  explicit Parser() noexcept;
  Parser(const ts::Parser &) = delete;
  Parser(ts::Parser &&) noexcept = default;
  ~Parser() noexcept = default;

  auto operator=(const ts::Parser &) -> ts::Parser & = delete;
  auto operator=(ts::Parser &&) noexcept -> ts::Parser & = default;

  auto Language() const noexcept -> ts::Language;
  auto SetLanguage(ts::Language &&language) const noexcept -> bool;
  [[nodiscard]] auto ParseString(ts::Tree &&old_tree,
                                 const std::string_view string) const noexcept
      -> ts::Tree;
  [[nodiscard]] auto
  ParseStringEncoding(ts::Tree &&old_tree, const std::string_view string,
                      const ts::InputEncoding encoding) const noexcept
      -> ts::Tree;

  static constexpr uint64_t kNoTimeout = 0;
  // If the `timeout_micros` is set to `kNoTimeout`, the timeout will be
  // disabled.
  auto SetTimeoutMicros(const uint64_t timeout_micros) const noexcept -> void;
  auto TimeoutMicros() const noexcept -> uint64_t;

  auto EnableCancellation() noexcept -> void;
  auto Cancel() noexcept -> void;
  auto DisableCancellation() noexcept -> void;

  auto SetLogger(ts::LoggerPtr &&logger) noexcept -> void;
  auto AccessLogger() const noexcept -> const ts::LoggerPtr &;
  [[nodiscard]] auto TakeLogger() noexcept -> ts::LoggerPtr;

  static constexpr int kCloseFileDescriptor = -1;
  // If the `file_descriptor` is set to `kCloseFileDescriptor`, the file
  // descriptor will be closed.
  auto PrintDotGraphs(const int file_descriptor) const noexcept -> void;

  auto IsNull() const noexcept -> bool;

private:
  ts::TSParserPtr ts_parser_;
  ts::CancellationFlagPtr cancellation_flag_;
  ts::LoggerPtr logger_;
};

} // namespace ts

#endif // CPP_TREE_SITTER_API_H
