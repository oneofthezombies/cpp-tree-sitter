#include "api.h"

#include <cassert>
#include <iostream>

using namespace ts;

// CStringDeleter
// --------

void ts::CStringDeleter::operator()(char *c_string_raw) const noexcept {
  if (c_string_raw == nullptr) {
    return;
  }
  free(c_string_raw);
}

// String
// --------

ts::String::String(ts::CStringPtr &&c_string) noexcept
    : c_string_{std::move(c_string)} {}

auto ts::String::IsNull() const noexcept -> bool {
  return c_string_.get() == nullptr;
}

auto ts::String::StringView() const noexcept -> std::string_view {
  if (IsNull()) {
    return std::string_view{};
  }
  return std::string_view{c_string_.get()};
}

// Point
// --------

ts::Point::Point(const TSPoint &ts_point) noexcept
    : TSPoint{ts_point.row, ts_point.column} {}

auto ts::Point::AsRaw() noexcept -> TSPoint & { return *this; }

auto ts::operator<<(std::ostream &os, const ts::Point &point)
    -> std::ostream & {
  os << "Point{";
  os << "row=" << point.row;
  os << ", ";
  os << "column=" << point.column;
  os << "}";
  return os;
}

// Node
// --------

ts::Node::Node(TSNode &&ts_node) noexcept : ts_node_{std::move(ts_node)} {}

auto ts::Node::StartByte() const noexcept -> uint32_t {
  return ts_node_start_byte(ts_node_);
}

auto ts::Node::StartPoint() const noexcept -> ts::Point {
  return ts::Point{ts_node_start_point(ts_node_)};
}

auto ts::Node::EndByte() const noexcept -> uint32_t {
  return ts_node_end_byte(ts_node_);
}

auto ts::Node::EndPoint() const noexcept -> ts::Point {
  return ts::Point{ts_node_end_point(ts_node_)};
}

auto ts::Node::Symbol() const noexcept -> ts::Symbol {
  return ts_node_symbol(ts_node_);
}

auto ts::Node::Type() const noexcept -> std::string_view {
  const auto type = ts_node_type(ts_node_);
  return std::string_view{type != nullptr ? type : ""};
}

auto ts::Node::GrammarSymbol() const noexcept -> ts::Symbol {
  return ts_node_grammar_symbol(ts_node_);
}

auto ts::Node::GrammarType() const noexcept -> std::string_view {
  const auto grammar_type = ts_node_grammar_type(ts_node_);
  return std::string_view{grammar_type != nullptr ? grammar_type : ""};
}

auto ts::Node::String() const noexcept -> ts::String {
  return ts::String{ts::CStringPtr{ts_node_string(ts_node_)}};
}

auto ts::Node::Eq(const ts::Node &other) const noexcept -> bool {
  return ts_node_eq(ts_node_, other.ts_node_);
}

auto ts::Node::IsNull() const noexcept -> bool {
  return ts_node_is_null(ts_node_);
}

auto ts::Node::IsExtra() const noexcept -> bool {
  return ts_node_is_extra(ts_node_);
}

auto ts::Node::IsNamed() const noexcept -> bool {
  return ts_node_is_named(ts_node_);
}

auto ts::Node::IsMissing() const noexcept -> bool {
  return ts_node_is_missing(ts_node_);
}

auto ts::Node::HasChanges() const noexcept -> bool {
  return ts_node_has_changes(ts_node_);
}

auto ts::Node::HasError() const noexcept -> bool {
  return ts_node_has_error(ts_node_);
}

auto ts::Node::IsError() const noexcept -> bool {
  return ts_node_is_error(ts_node_);
}

auto ts::Node::DescendantCount() const noexcept -> uint32_t {
  return ts_node_descendant_count(ts_node_);
}

auto ts::Node::ParseState() const noexcept -> ts::StateId {
  return ts_node_parse_state(ts_node_);
}

auto ts::Node::NextParseState() const noexcept -> ts::StateId {
  return ts_node_next_parse_state(ts_node_);
}

auto ts::Node::Parent() const noexcept -> ts::Node {
  return ts::Node{ts_node_parent(ts_node_)};
}

auto ts::Node::Child(const uint32_t index) const noexcept -> ts::Node {
  return ts::Node{ts_node_child(ts_node_, index)};
}

auto ts::Node::NamedChild(const uint32_t index) const noexcept -> ts::Node {
  return ts::Node{ts_node_named_child(ts_node_, index)};
}

auto ts::Node::ChildByFieldId(const FieldId field_id) const noexcept
    -> ts::Node {
  return ts::Node{ts_node_child_by_field_id(ts_node_, field_id)};
}

auto ts::Node::FieldNameForChild(const uint32_t index) const noexcept
    -> std::string_view {
  const auto field_name = ts_node_field_name_for_child(ts_node_, index);
  return std::string_view{field_name != nullptr ? field_name : ""};
}

auto ts::Node::ChildByFieldName(
    const std::string_view field_name) const noexcept -> ts::Node {
  return ts::Node{ts_node_child_by_field_name(ts_node_, field_name.data(),
                                              field_name.size())};
}

auto ts::Node::ChildCount() const noexcept -> uint32_t {
  return ts_node_child_count(ts_node_);
}

auto ts::Node::NamedChildCount() const noexcept -> uint32_t {
  return ts_node_named_child_count(ts_node_);
}

auto ts::Node::NextSibling() const noexcept -> ts::Node {
  return ts::Node{ts_node_next_sibling(ts_node_)};
}

auto ts::Node::NextNamedSibling() const noexcept -> ts::Node {
  return ts::Node{ts_node_next_named_sibling(ts_node_)};
}

auto ts::Node::PrevSibling() const noexcept -> ts::Node {
  return ts::Node{ts_node_prev_sibling(ts_node_)};
}

auto ts::Node::PrevNamedSibling() const noexcept -> ts::Node {
  return ts::Node{ts_node_prev_named_sibling(ts_node_)};
}

auto ts::Node::FirstChildForByte(const uint32_t byte) const noexcept
    -> ts::Node {
  return ts::Node{ts_node_first_child_for_byte(ts_node_, byte)};
}

auto ts::Node::FirstNamedChildForByte(const uint32_t byte) const noexcept
    -> ts::Node {
  return ts::Node{ts_node_first_named_child_for_byte(ts_node_, byte)};
}

auto ts::Node::DescendantForByteRange(const uint32_t start,
                                      const uint32_t end) const noexcept
    -> ts::Node {
  return ts::Node{ts_node_descendant_for_byte_range(ts_node_, start, end)};
}

auto ts::Node::NamedDescendantForByteRange(const uint32_t start,
                                           const uint32_t end) const noexcept
    -> ts::Node {
  return ts::Node{
      ts_node_named_descendant_for_byte_range(ts_node_, start, end)};
}

auto ts::Node::DescendantForPointRange(const ts::Point start,
                                       const ts::Point end) const noexcept
    -> ts::Node {
  return ts::Node{ts_node_descendant_for_point_range(ts_node_, start, end)};
}

auto ts::Node::NamedDescendantForPointRange(const ts::Point start,
                                            const ts::Point end) const noexcept
    -> ts::Node {
  return ts::Node{
      ts_node_named_descendant_for_point_range(ts_node_, start, end)};
}

auto ts::Node::AsRaw() noexcept -> TSNode & { return ts_node_; }

auto ts::operator<<(std::ostream &os, const ts::Node &node) -> std::ostream & {
  os << "Node{";
  os << "start_byte=" << node.StartByte();
  os << ", ";
  os << "start_point=" << node.StartPoint();
  os << ", ";
  os << "end_byte=" << node.EndByte();
  os << ", ";
  os << "end_point=" << node.EndPoint();
  os << ", ";
  os << "symbol=" << node.Symbol();
  os << ", ";
  os << "type=" << node.Type();
  os << ", ";
  os << "grammar_symbol=" << node.GrammarSymbol();
  os << ", ";
  os << "grammar_type=" << node.GrammarType();
  os << ", ";
  os << "is_null=" << node.IsNull();
  os << ", ";
  os << "is_extra=" << node.IsExtra();
  os << ", ";
  os << "is_named=" << node.IsNamed();
  os << ", ";
  os << "is_missing=" << node.IsMissing();
  os << ", ";
  os << "has_changes=" << node.HasChanges();
  os << ", ";
  os << "has_error=" << node.HasError();
  os << ", ";
  os << "is_error=" << node.IsError();
  os << ", ";
  os << "descendant_count=" << node.DescendantCount();
  os << ", ";
  os << "parse_state=" << node.ParseState();
  os << ", ";
  os << "next_parse_state=" << node.NextParseState();
  os << "}";
  return os;
}

// TSTreeDeleter
// --------

void ts::TSTreeDeleter::operator()(TSTree *ts_tree_raw) const noexcept {
  ts_tree_delete(ts_tree_raw);
}

// Tree
// --------

ts::Tree::Tree(ts::TSTreePtr &&ts_tree) noexcept
    : ts_tree_{std::move(ts_tree)} {}

auto ts::Tree::IsNull() const noexcept -> bool {
  return ts_tree_.get() == nullptr;
}

auto ts::Tree::IntoRaw() noexcept -> ts::TSTreePtr {
  return std::move(ts_tree_);
}

auto ts::Tree::RootNode() const noexcept -> ts::Node {
  assert(!IsNull() && "Tree::RootNode: tree is null");
  return ts::Node{ts_tree_root_node(ts_tree_.get())};
}

auto ts::Tree::PrintDotGraph(const int file_descriptor) const noexcept -> void {
  assert(!IsNull() && "Tree::PrintDotGraph: tree is null");
  ts_tree_print_dot_graph(ts_tree_.get(), file_descriptor);
}

auto printRec(std::ostream &os, const ts::Node &parent_node,
              const uint32_t current_index, const uint32_t level) -> void {
  for (uint32_t i = 0; i < level; ++i) {
    os << "  ";
  }
  const auto field_name = parent_node.FieldNameForChild(current_index);
  os << "Child{";
  os << "index=";
  os << current_index;
  os << ", ";
  if (!field_name.empty()) {
    os << "field_name=";
    os << field_name;
    os << ", ";
  }
  os << "node=";
  const auto current_node = parent_node.Child(current_index);
  os << current_node << "}\n";
  const auto child_count = current_node.ChildCount();
  for (uint32_t i = 0; i < child_count; ++i) {
    printRec(os, current_node, i, level + 1);
  }
};

auto ts::operator<<(std::ostream &os, const ts::Tree &tree) -> std::ostream & {
  os << "Tree{";
  if (tree.IsNull()) {
    os << "null";
    os << "}";
    return os;
  }

  os << "root_node=";
  const auto root_node = tree.RootNode();
  os << root_node << "}";
  const auto child_count = root_node.ChildCount();
  if (child_count == 0) {
    return os;
  }

  os << '\n';
  for (uint32_t i = 0; i < child_count; ++i) {
    printRec(os, root_node, i, 1);
  }

  return os;
}

auto ts::Tree::Null() noexcept -> ts::Tree {
  return ts::Tree{ts::TSTreePtr{nullptr}};
}

// Logger
// --------

auto LoggerLog(void *logger, ts::LogType log_type, const char *buffer) noexcept
    -> void {
  assert(logger != nullptr && "logger_log: logger is null");
  static_cast<ts::Logger *>(logger)->Log(log_type,
                                         buffer != nullptr ? buffer : "");
}

ts::Logger::Logger() noexcept : ts_logger_{this, LoggerLog} {}

auto ts::Logger::AsRaw() noexcept -> TSLogger & { return ts_logger_; }

auto NullTsLogger() noexcept -> TSLogger { return TSLogger{nullptr, nullptr}; }

// ConsoleLogger
// --------

auto ts::ConsoleLogger::Log(const ts::LogType log_type,
                            const std::string_view buffer) const noexcept
    -> void {
  std::cout << "Log{";
  std::cout << "log_type=";
  switch (log_type) {
  case TSLogTypeParse:
    std::cout << "Parse";
    break;
  case TSLogTypeLex:
    std::cout << "Lex";
    break;
  }
  std::cout << ", ";
  std::cout << "buffer=\"";
  std::cout << buffer;
  std::cout << "\"}\n";
}

// Language
// --------

ts::Language::Language(const TSLanguage *const ts_language) noexcept
    : ts_language_{ts_language} {
  assert(ts_language != nullptr && "Language::Language: ts_language is null");
}

auto ts::Language::SymbolCount() const noexcept -> uint32_t {
  return ts_language_symbol_count(ts_language_);
}

auto ts::Language::StateCount() const noexcept -> uint32_t {
  return ts_language_state_count(ts_language_);
}

auto ts::Language::SymbolName(const ts::Symbol symbol) const noexcept
    -> std::string_view {
  const auto symbol_name = ts_language_symbol_name(ts_language_, symbol);
  return std::string_view{symbol_name != nullptr ? symbol_name : ""};
}

auto ts::Language::SymbolForName(const std::string_view name,
                                 const bool is_named) const noexcept
    -> ts::Symbol {
  return ts_language_symbol_for_name(ts_language_, name.data(), name.size(),
                                     is_named);
}

auto ts::Language::FieldCount() const noexcept -> uint32_t {
  return ts_language_field_count(ts_language_);
}

auto ts::Language::FieldNameForId(const ts::FieldId field_id) const noexcept
    -> std::string_view {
  const auto field_name = ts_language_field_name_for_id(ts_language_, field_id);
  return std::string_view{field_name != nullptr ? field_name : ""};
}

auto ts::Language::FieldIdForName(const std::string_view name) const noexcept
    -> ts::FieldId {
  return ts_language_field_id_for_name(ts_language_, name.data(), name.size());
}

auto ts::Language::SymbolType(const ts::Symbol symbol) const noexcept
    -> ts::SymbolType {
  return ts_language_symbol_type(ts_language_, symbol);
}

auto ts::Language::Version() const noexcept -> uint32_t {
  return ts_language_version(ts_language_);
}

auto ts::Language::NextState(const ts::StateId state,
                             const ts::Symbol symbol) const noexcept
    -> ts::StateId {
  return ts_language_next_state(ts_language_, state, symbol);
}

auto ts::Language::AsRaw() const noexcept -> const TSLanguage *const {
  return ts_language_;
}

// TSParserDeleter
// --------

void ts::TSParserDeleter::operator()(TSParser *ts_parser_raw) const noexcept {
  ts_parser_print_dot_graphs(ts_parser_raw, Parser::kCloseFileDescriptor);
  ts_parser_delete(ts_parser_raw);
}

// Parser
// --------

ts::Parser::Parser() noexcept : ts_parser_{ts_parser_new()} {}

auto ts::Parser::Language() const noexcept -> ts::Language {
  assert(!IsNull() && "Parser::Language: parser is null");
  return ts::Language{ts_parser_language(ts_parser_.get())};
}

auto ts::Parser::SetLanguage(ts::Language &&language) const noexcept -> bool {
  assert(!IsNull() && "Parser::SetLanguage: parser is null");
  return ts_parser_set_language(ts_parser_.get(), language.AsRaw());
}

auto ts::Parser::ParseString(ts::Tree &&old_tree,
                             const std::string_view string) const noexcept
    -> ts::Tree {
  assert(!IsNull() && "Parser::ParseString: parser is null");
  const auto old_tree_raw = old_tree.IntoRaw();
  const auto new_tree = ts_parser_parse_string(
      ts_parser_.get(), old_tree_raw.get(), string.data(), string.size());
  return ts::Tree{ts::TSTreePtr{new_tree}};
}

auto ts::Parser::ParseStringEncoding(
    ts::Tree &&old_tree, const std::string_view string,
    const ts::InputEncoding encoding) const noexcept -> ts::Tree {
  assert(!IsNull() && "Parser::ParseStringEncoding: parser is null");
  const auto old_tree_raw = old_tree.IntoRaw();
  const auto new_tree =
      ts_parser_parse_string_encoding(ts_parser_.get(), old_tree_raw.get(),
                                      string.data(), string.size(), encoding);
  return Tree{TSTreePtr{new_tree}};
}

auto ts::Parser::SetTimeoutMicros(const uint64_t timeout_micros) const noexcept
    -> void {
  assert(!IsNull() && "Parser::SetTimeoutMicros: parser is null");
  ts_parser_set_timeout_micros(ts_parser_.get(), timeout_micros);
}

auto ts::Parser::TimeoutMicros() const noexcept -> uint64_t {
  assert(!IsNull() && "Parser::TimeoutMicros: parser is null");
  return ts_parser_timeout_micros(ts_parser_.get());
}

auto ts::Parser::EnableCancellation() noexcept -> void {
  assert(!IsNull() && "Parser::EnableCancellation: parser is null");
  if (cancellation_flag_.get() != nullptr) {
    return;
  }
  cancellation_flag_ = std::make_unique<size_t>(0);
  ts_parser_set_cancellation_flag(ts_parser_.get(), cancellation_flag_.get());
}

auto ts::Parser::Cancel() noexcept -> void {
  assert(!IsNull() && "Parser::Cancel: parser is null");
  assert(cancellation_flag_.get() != nullptr &&
         "Parser::Cancel: cancellation_flag is null. Please enable "
         "cancellation before canceling.");
  *cancellation_flag_ = 1;
}

auto ts::Parser::DisableCancellation() noexcept -> void {
  assert(!IsNull() && "Parser::DisableCancellation: parser is null");
  if (cancellation_flag_.get() == nullptr) {
    return;
  }
  ts_parser_set_cancellation_flag(ts_parser_.get(), nullptr);
  cancellation_flag_.reset();
}

auto ts::Parser::SetLogger(ts::LoggerPtr &&logger) noexcept -> void {
  assert(!IsNull() && "Parser::SetLogger: parser is null");
  assert(logger.get() != nullptr && "Parser::SetLogger: logger is null");
  logger_ = std::move(logger);
  ts_parser_set_logger(ts_parser_.get(), logger_->AsRaw());
}

auto ts::Parser::AccessLogger() const noexcept -> const ts::LoggerPtr & {
  assert(!IsNull() && "Parser::AccessLogger: parser is null");
  assert(logger_.get() != nullptr && "Parser::AccessLogger: logger is null");
  return logger_;
}

auto ts::Parser::TakeLogger() noexcept -> ts::LoggerPtr {
  assert(!IsNull() && "Parser::TakeLogger: parser is null");
  assert(logger_.get() != nullptr && "Parser::TakeLogger: logger is null");
  ts_parser_set_logger(ts_parser_.get(), NullTsLogger());
  return std::move(logger_);
}

auto ts::Parser::PrintDotGraphs(const int file_descriptor) const noexcept
    -> void {
  assert(!IsNull() && "Parser::PrintDotGraphs: parser is null");
  assert(((file_descriptor != 0) && (file_descriptor != 1) &&
          (file_descriptor != 2)) &&
         "Parser::PrintDotGraphs: You cannot use the stdin/stdout/stderr "
         "file descriptor "
         "directly. Close file descriptor when parser is deleted. Please use "
         "it after duplication.");
  ts_parser_print_dot_graphs(ts_parser_.get(), file_descriptor);
}

auto ts::Parser::IsNull() const noexcept -> bool {
  return ts_parser_.get() == nullptr;
}
