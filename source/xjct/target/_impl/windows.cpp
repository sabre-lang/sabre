/// XJCT Modules
#include "xjct/target/windows.hpp"
#include "xjct/forward/vendors.hpp"

//  PRIVATE METHODS  //

bool XJCT::Target::Windows::m_imbue(Archive::Binary &binary, const Imbue::Options &options) const noexcept {
  // construct a stream to be used instead of the buffer
  auto stream = $::Unique::New<LIEF::SpanStream>(binary.data(), binary.size());

  auto parser = LIEF::PE::Parser::parse(std::move(stream));
  if (parser == nullptr) return false; // failed building

  // ensure that there are actually some resources available
  if (!parser->has_resources()) return false;

  LIEF::PE::ResourceNode *id_node = nullptr;
  LIEF::PE::ResourceNode *rcdata_node = nullptr;
  LIEF::PE::ResourceNode *resources = parser->resources();

  // first-level => TYPE (ResourceDirectory node)
  static constexpr auto rcdata_node_id = static_cast<uint32_t>(LIEF::PE::ResourcesManager::TYPE::RCDATA);
  auto rcdata_node_iter =
      std::find_if(resources->childs().begin(), resources->childs().end(), [](const LIEF::PE::ResourceNode &node) {
        return node.id() == rcdata_node_id;
      });

  if (rcdata_node_iter != resources->childs().end()) rcdata_node = &*rcdata_node_iter;
  else rcdata_node = &resources->add_child(LIEF::PE::ResourceDirectory(rcdata_node_id));

  // second-level => ID (ResourceDirectory node)
  auto id_node_iter = std::find_if(
      rcdata_node->childs().begin(), rcdata_node->childs().end(), [options](const LIEF::PE::ResourceNode &node) {
        return node.utf8_name() == options.name;
      }
  );

  if (id_node_iter != rcdata_node->childs().end()) id_node = &*id_node_iter; // update the node now
  else (id_node = &rcdata_node->add_child(LIEF::PE::ResourceDirectory(0x80000000)))->name(options.name);

  // third-level => Lang (ResourceData node)
  if (id_node->childs() != id_node->childs().end()) return false;

  // add the necessary data to be contained now
  Blob::Bytes content = {options.blob.begin(), options.blob.end()};
  id_node->add_child(LIEF::PE::ResourceData(content)); // rebuild now

  // prepare the configuration for building
  LIEF::PE::Builder::config_t configuration = {
      .imports = false,
      .resources = true,
      .relocations = false,
      .tls = false,
      .overlay = false,
      .dos_stub = true,
  };

  // and update the underlying binary now
  auto builder = LIEF::PE::Builder(*parser, configuration);
  return builder.build(), binary.update(builder.get_build());
}
