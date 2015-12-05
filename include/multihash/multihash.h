#pragma once

#include <string>
#include <istream>
#include <set>
#include <vector>
#include <exception>
#include <memory>
#include <cstdint>

namespace multihash
{

/** Base exception for the multihash library */
class Exception : public std::exception
{
public:
    explicit Exception(std::string message);
    const char* what() const noexcept override;
private:
    std::string message_;
};

/** Thrown when an invalid hash name or type is used */
struct InvalidHashException : public Exception
{
    explicit InvalidHashException(const std::string& message);
};

typedef std::vector<unsigned char> Buffer;

/** Code values of supported hash types */
enum class HashCode : unsigned char
{
    SHA1        = 0x11, //"sha1"
    SHA2_256    = 0x12, //"sha2-256"
    SHA2_512    = 0x13, //"sha2-512"
    SHA3        = 0x14, //"sha3"
    BLAKE2B     = 0x40, //"blake2b"
    BLAKE2S     = 0x41  //"blake2s"
};

/** Interface for a hash Type */
class HashType
{
public:
    HashType(const std::string& name);
    HashType(HashCode code);
    HashType(HashType&& rhs) noexcept;
    HashType(const HashType& rhs);

    const std::string& name() const;
    HashCode code() const;
    size_t size() const; /**< Default length of the hash type */

    bool operator==(const HashType& rhs) const;
    bool operator<(const HashType& rhs)  const;
private:
    class Impl;
    std::shared_ptr<Impl> pImpl;
};
std::set<HashType> hashTypes();

/** Interface for a hash digest with encapsulated type */
class Hash
{
public:
    Hash(HashType type, const Buffer& digest);
    Hash(Hash&& rhs) noexcept;
    Hash(const Hash& rhs);
    ~Hash();

    const HashType& type() const;
    const Buffer& digest() const;

    bool operator==(const Hash& rhs) const;
private:
    class Impl;
    std::shared_ptr<Impl> pImpl;
};

/** Functor which hashes an input stream with a given hash type */
class HashFunction
{
public:
    explicit HashFunction(const HashType& hash_type);
    HashFunction(HashFunction&& rhs) noexcept;
    ~HashFunction();

    Hash operator() (std::istream& input); 
    const HashType& type() const;
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

/** Functor which packs a hash into a buffer */
class BufferEncoder
{
public:
    BufferEncoder();
    ~BufferEncoder();

    Buffer operator() (const Hash& hash);
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

/** Functor which can extract a hash from a raw buffer */
class BufferDecoder
{
public:
    BufferDecoder();
    ~BufferDecoder();

    Hash operator() (const Buffer& input);
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} //namespace multihash

std::ostream& operator<<(std::ostream& os, const multihash::HashCode& hash_code);
std::ostream& operator<<(std::ostream& os, const multihash::HashType& hash_type);
std::ostream& operator<<(std::ostream& os, const multihash::Hash& hash);
