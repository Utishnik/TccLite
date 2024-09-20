#ifndef STRMAPENT
#define STRMAPENT
#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace LLVM_CLANG
{
    class StringMapEntryBase {
        size_t keyLength;

        public:
        explicit StringMapEntryBase(size_t keyLength) : keyLength(keyLength) {}

        size_t getKeyLength() const { return keyLength; }

        protected:
        /// Helper to tail-allocate \p Key. It'd be nice to generalize this so it
        /// could be reused elsewhere, maybe even taking an llvm::function_ref to
        /// type-erase the allocator and put it in a source file.
        template <typename AllocatorTy>
        static void *allocateWithKey(size_t EntrySize, size_t EntryAlign,
                                    std::string Key, AllocatorTy &Allocator);
    };

    template <typename AllocatorTy>
    void *StringMapEntryBase::allocateWithKey(size_t EntrySize, size_t EntryAlign,
                                            std::string Key,
                                            AllocatorTy &Allocator) {
        size_t KeyLength = Key.size();

        // Allocate a new item with space for the string at the end and a null
        // terminator.
        size_t AllocSize = EntrySize + KeyLength + 1;
        void *Allocation = Allocator.Allocate(AllocSize, EntryAlign);
        assert(Allocation && "Unhandled out-of-memory");

        // Copy the string information.
        char *Buffer = reinterpret_cast<char *>(Allocation) + EntrySize;
        if (KeyLength > 0)
            ::memcpy(Buffer, Key.data(), KeyLength);
        Buffer[KeyLength] = 0; // Null terminate for convenience of clients.
        return Allocation;
    }


    struct nullopt_t
    {
        // Do not user-declare default constructor at all for
        // optional_value = {} syntax to work.
        // nullopt_t() = delete;

        // Used for constructing nullopt.
        enum class _Construct { _Token };

        // Must be constexpr for nullopt_t to be literal.
        explicit constexpr nullopt_t(_Construct) noexcept { }
    };
    inline constexpr nullopt_t nullopt { nullopt_t::_Construct::_Token };

    template <typename ValueTy>
    class StringMapEntryStorage : public StringMapEntryBase {
    public:
    ValueTy second;

    explicit StringMapEntryStorage(size_t keyLength)
        : StringMapEntryBase(keyLength), second() {}
    template <typename... InitTy>
    StringMapEntryStorage(size_t keyLength, InitTy &&...initVals)
        : StringMapEntryBase(keyLength),
            second(std::forward<InitTy>(initVals)...) {}
    StringMapEntryStorage(StringMapEntryStorage &e) = delete;

    const ValueTy &getValue() const { return second; }
    ValueTy &getValue() { return second; }

    void setValue(const ValueTy &V) { second = V; }
    };

    template <>
    class StringMapEntryStorage<LLVM_CLANG::nullopt_t> : public StringMapEntryBase {
    public:
        explicit StringMapEntryStorage(size_t keyLength,
                                        LLVM_CLANG::nullopt_t = LLVM_CLANG::nullopt)
            : StringMapEntryBase(keyLength) {}
        StringMapEntryStorage(StringMapEntryStorage &entry) = delete;

        LLVM_CLANG::nullopt_t getValue() const { return LLVM_CLANG::nullopt; }
    };
}

#endif