#define DUCKDB_EXTENSION_MAIN

#include "duckdb_gemmi_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/function/scalar_function.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void DuckdbGemmiScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "DuckdbGemmi " + name.GetString() + " 🐥");
	});
}

inline void DuckdbGemmiOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "DuckdbGemmi " + name.GetString() + ", my linked OpenSSL version is " +
		                                           OPENSSL_VERSION_TEXT);
	});
}

static void LoadInternal(ExtensionLoader &loader) {
	// Register a scalar function
	auto duckdb_gemmi_scalar_function = ScalarFunction("duckdb_gemmi", {LogicalType::VARCHAR}, LogicalType::VARCHAR, DuckdbGemmiScalarFun);
	loader.RegisterFunction(duckdb_gemmi_scalar_function);

	// Register another scalar function
	auto duckdb_gemmi_openssl_version_scalar_function = ScalarFunction("duckdb_gemmi_openssl_version", {LogicalType::VARCHAR},
	                                                            LogicalType::VARCHAR, DuckdbGemmiOpenSSLVersionScalarFun);
	loader.RegisterFunction(duckdb_gemmi_openssl_version_scalar_function);
}

void DuckdbGemmiExtension::Load(ExtensionLoader &loader) {
	LoadInternal(loader);
}
std::string DuckdbGemmiExtension::Name() {
	return "duckdb_gemmi";
}

std::string DuckdbGemmiExtension::Version() const {
#ifdef EXT_VERSION_DUCKDB_GEMMI
	return EXT_VERSION_DUCKDB_GEMMI;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_CPP_EXTENSION_ENTRY(duckdb_gemmi, loader) {
	duckdb::LoadInternal(loader);
}
}
