#ifndef LLVMITABLE
#define LLVMITABLE
#define LLVM_PREFERRED_TYPE(T)
#include "tools.h"
#include "tokens/TokenKinds.h"

namespace LLVM_CLANG
{
    class DeclarationName;
    class DeclarationNameTable;
    class IdentifierInfo;
    class LangOptions;
    class MultiKeywordSelector;
    class SourceLocation;
    static constexpr int InterestingIdentifierBits = 16;


    enum class ReservedIdentifierStatus {
        NotReserved = 0,
        StartsWithUnderscoreAtGlobalScope,
        StartsWithUnderscoreAndIsExternC,
        StartsWithDoubleUnderscore,
        StartsWithUnderscoreFollowedByCapitalLetter,
        ContainsDoubleUnderscore,
    };

    enum class ReservedLiteralSuffixIdStatus {
        NotReserved = 0,
        NotStartsWithUnderscore,
        ContainsDoubleUnderscore,
    };


    /// Determine whether an identifier is reserved for use as a name at global
    /// scope. Such identifiers might be implementation-specific global functions
    /// or variables.
    inline bool isReservedAtGlobalScope(ReservedIdentifierStatus Status) 
    {
        return Status != ReservedIdentifierStatus::NotReserved;
    }

    /// Determine whether an identifier is reserved in all contexts. Such
    /// identifiers might be implementation-specific keywords or macros, for
    /// example.
    inline bool isReservedInAllContexts(ReservedIdentifierStatus Status) 
    {
        return Status != ReservedIdentifierStatus::NotReserved &&
        Status != ReservedIdentifierStatus::StartsWithUnderscoreAtGlobalScope &&
        Status != ReservedIdentifierStatus::StartsWithUnderscoreAndIsExternC;
    }

    class alignas(IdentifierInfoAlignment) IdentifierInfo 
    {
        friend class IdentifierTable;

        // Front-end token ID or tok::identifier.
        LLVM_PREFERRED_TYPE(tok::TokenKind)
        unsigned TokenID : 9;

        LLVM_PREFERRED_TYPE(InterestingIdentifier)
        unsigned InterestingIdentifierID : InterestingIdentifierBits;

        // True if there is a #define for this.
        LLVM_PREFERRED_TYPE(bool)
        unsigned HasMacro : 1;

        // True if there was a #define for this.
        LLVM_PREFERRED_TYPE(bool)
        unsigned HadMacro : 1;

        // True if the identifier is a language extension.
        LLVM_PREFERRED_TYPE(bool)
        unsigned IsExtension : 1;

        // True if the identifier is a keyword in a newer or proposed Standard.
        LLVM_PREFERRED_TYPE(bool)
        unsigned IsFutureCompatKeyword : 1;

        // True if the identifier is poisoned.
        LLVM_PREFERRED_TYPE(bool)
        unsigned IsPoisoned : 1;

        // True if the identifier is a C++ operator keyword.
        LLVM_PREFERRED_TYPE(bool)
        unsigned IsCPPOperatorKeyword : 1;

        // Internal bit set by the member function RecomputeNeedsHandleIdentifier.
        // See comment about RecomputeNeedsHandleIdentifier for more info.
        LLVM_PREFERRED_TYPE(bool)
        unsigned NeedsHandleIdentifier : 1;

        // True if the identifier was loaded (at least partially) from an AST file.
        LLVM_PREFERRED_TYPE(bool)
        unsigned IsFromAST : 1;

        // True if the identifier has changed from the definition
        // loaded from an AST file.
        LLVM_PREFERRED_TYPE(bool)
        unsigned ChangedAfterLoad : 1;

        // True if the identifier's frontend information has changed from the
        // definition loaded from an AST file.
        LLVM_PREFERRED_TYPE(bool)
        unsigned FEChangedAfterLoad : 1;

        // True if revertTokenIDToIdentifier was called.
        LLVM_PREFERRED_TYPE(bool)
        unsigned RevertedTokenID : 1;

        // True if there may be additional information about
        // this identifier stored externally.
        LLVM_PREFERRED_TYPE(bool)
        unsigned OutOfDate : 1;

        // True if this is the 'import' contextual keyword.
        LLVM_PREFERRED_TYPE(bool)
        unsigned IsModulesImport : 1;

        // True if this is a mangled OpenMP variant name.
        LLVM_PREFERRED_TYPE(bool)
        unsigned IsMangledOpenMPVariantName : 1;

        // True if this is a deprecated macro.
        LLVM_PREFERRED_TYPE(bool)
        unsigned IsDeprecatedMacro : 1;

        // True if this macro is unsafe in headers.
        LLVM_PREFERRED_TYPE(bool)
        unsigned IsRestrictExpansion : 1;

        // True if this macro is final.
        LLVM_PREFERRED_TYPE(bool)
        unsigned IsFinal : 1;

        // 22 bits left in a 64-bit word.

        // Managed by the language front-end.
        void *FETokenInfo = nullptr;

        LLVM_CLANG::StringMapEntry<IdentifierInfo *> *Entry = nullptr;

        IdentifierInfo()
            : TokenID(tok::identifier),
                InterestingIdentifierID(llvm::to_underlying(
                    InterestingIdentifier::NotInterestingIdentifier)),
                HasMacro(false), HadMacro(false), IsExtension(false),
                IsFutureCompatKeyword(false), IsPoisoned(false),
                IsCPPOperatorKeyword(false), NeedsHandleIdentifier(false),
                IsFromAST(false), ChangedAfterLoad(false), FEChangedAfterLoad(false),
                RevertedTokenID(false), OutOfDate(false), IsModulesImport(false),
                IsMangledOpenMPVariantName(false), IsDeprecatedMacro(false),
                IsRestrictExpansion(false), IsFinal(false) {}

        public:
        IdentifierInfo(const IdentifierInfo &) = delete;
        IdentifierInfo &operator=(const IdentifierInfo &) = delete;
        IdentifierInfo(IdentifierInfo &&) = delete;
        IdentifierInfo &operator=(IdentifierInfo &&) = delete;

        /// Return true if this is the identifier for the specified string.
        ///
        /// This is intended to be used for string literals only: II->isStr("foo").
        template <std::size_t StrLen>
        bool isStr(const char (&Str)[StrLen]) const {
            return getLength() == StrLen-1 &&
                memcmp(getNameStart(), Str, StrLen-1) == 0;
        }

        /// Return true if this is the identifier for the specified StringRef.
        bool isStr(std::string Str) const {
            std::string ThisStr(getNameStart(), getLength());
            return ThisStr == Str;
        }

        /// Return the beginning of the actual null-terminated string for this
        /// identifier.
        const char *getNameStart() const { return Entry->getKeyData(); }

        /// Efficiently return the length of this identifier info.
        unsigned getLength() const { return Entry->getKeyLength(); }

        /// Return the actual identifier string.
        StringRef getName() const {
            return StringRef(getNameStart(), getLength());
        }

        /// Return true if this identifier is \#defined to some other value.
        /// \note The current definition may be in a module and not currently visible.
        bool hasMacroDefinition() const {
            return HasMacro;
        }
        void setHasMacroDefinition(bool Val) {
            if (HasMacro == Val) return;

            HasMacro = Val;
            if (Val) {
            NeedsHandleIdentifier = true;
            HadMacro = true;
            } else {
            // If this is a final macro, make the deprecation and header unsafe bits
            // stick around after the undefinition so they apply to any redefinitions.
            if (!IsFinal) {
                // Because calling the setters of these calls recomputes, just set them
                // manually to avoid recomputing a bunch of times.
                IsDeprecatedMacro = false;
                IsRestrictExpansion = false;
            }
            RecomputeNeedsHandleIdentifier();
            }
        }
        /// Returns true if this identifier was \#defined to some value at any
        /// moment. In this case there should be an entry for the identifier in the
        /// macro history table in Preprocessor.
        bool hadMacroDefinition() const {
            return HadMacro;
        }

        bool isDeprecatedMacro() const { return IsDeprecatedMacro; }

        void setIsDeprecatedMacro(bool Val) {
            if (IsDeprecatedMacro == Val)
            return;
            IsDeprecatedMacro = Val;
            if (Val)
            NeedsHandleIdentifier = true;
            else
            RecomputeNeedsHandleIdentifier();
        }

        bool isRestrictExpansion() const { return IsRestrictExpansion; }

        void setIsRestrictExpansion(bool Val) {
            if (IsRestrictExpansion == Val)
            return;
            IsRestrictExpansion = Val;
            if (Val)
            NeedsHandleIdentifier = true;
            else
            RecomputeNeedsHandleIdentifier();
        }

        bool isFinal() const { return IsFinal; }

        void setIsFinal(bool Val) { IsFinal = Val; }

        /// If this is a source-language token (e.g. 'for'), this API
        /// can be used to cause the lexer to map identifiers to source-language
        /// tokens.
        tok::TokenKind getTokenID() const { return (tok::TokenKind)TokenID; }

        /// True if revertTokenIDToIdentifier() was called.
        bool hasRevertedTokenIDToIdentifier() const { return RevertedTokenID; }

        /// Revert TokenID to tok::identifier; used for GNU libstdc++ 4.2
        /// compatibility.
        ///
        /// TokenID is normally read-only but there are 2 instances where we revert it
        /// to tok::identifier for libstdc++ 4.2. Keep track of when this happens
        /// using this method so we can inform serialization about it.
        void revertTokenIDToIdentifier() {
            assert(TokenID != tok::identifier && "Already at tok::identifier");
            TokenID = tok::identifier;
            RevertedTokenID = true;
        }
        void revertIdentifierToTokenID(tok::TokenKind TK) {
            assert(TokenID == tok::identifier && "Should be at tok::identifier");
            TokenID = TK;
            RevertedTokenID = false;
        }

        /// Return the preprocessor keyword ID for this identifier.
        ///
        /// For example, "define" will return tok::pp_define.
        tok::PPKeywordKind getPPKeywordID() const;

        /// Return the Objective-C keyword ID for the this identifier.
        ///
        /// For example, 'class' will return tok::objc_class if ObjC is enabled.
        tok::ObjCKeywordKind getObjCKeywordID() const {
            assert(0 == llvm::to_underlying(InterestingIdentifier::objc_not_keyword));
            auto Value = static_cast<InterestingIdentifier>(InterestingIdentifierID);
            if (Value < InterestingIdentifier::NUM_OBJC_KEYWORDS)
            return static_cast<tok::ObjCKeywordKind>(InterestingIdentifierID);
            return tok::objc_not_keyword;
        }
        void setObjCKeywordID(tok::ObjCKeywordKind ID) {
            assert(0 == llvm::to_underlying(InterestingIdentifier::objc_not_keyword));
            InterestingIdentifierID = ID;
            assert(getObjCKeywordID() == ID && "ID too large for field!");
        }

        /// Return a value indicating whether this is a builtin function.
        unsigned getBuiltinID() const {
            auto Value = static_cast<InterestingIdentifier>(InterestingIdentifierID);
            if (Value >
                    InterestingIdentifier::NUM_OBJC_KEYWORDS_AND_NOTABLE_IDENTIFIERS &&
                Value != InterestingIdentifier::NotInterestingIdentifier) {
            auto FirstBuiltin =
                llvm::to_underlying(InterestingIdentifier::NotBuiltin);
            return static_cast<Builtin::ID>(InterestingIdentifierID - FirstBuiltin);
            }
            return Builtin::ID::NotBuiltin;
        }
        void setBuiltinID(unsigned ID) {
            assert(ID != Builtin::ID::NotBuiltin);
            auto FirstBuiltin = llvm::to_underlying(InterestingIdentifier::NotBuiltin);
            InterestingIdentifierID = ID + FirstBuiltin;
            assert(getBuiltinID() == ID && "ID too large for field!");
        }
        void clearBuiltinID() {
            InterestingIdentifierID =
                llvm::to_underlying(InterestingIdentifier::NotInterestingIdentifier);
        }

        tok::NotableIdentifierKind getNotableIdentifierID() const {
            auto Value = static_cast<InterestingIdentifier>(InterestingIdentifierID);
            if (Value > InterestingIdentifier::NUM_OBJC_KEYWORDS &&
                Value <
                    InterestingIdentifier::NUM_OBJC_KEYWORDS_AND_NOTABLE_IDENTIFIERS) {
            auto FirstNotableIdentifier =
                1 + llvm::to_underlying(InterestingIdentifier::NUM_OBJC_KEYWORDS);
            return static_cast<tok::NotableIdentifierKind>(InterestingIdentifierID -
                                                            FirstNotableIdentifier);
            }
            return tok::not_notable;
        }
        void setNotableIdentifierID(unsigned ID) {
            assert(ID != tok::not_notable);
            auto FirstNotableIdentifier =
                1 + llvm::to_underlying(InterestingIdentifier::NUM_OBJC_KEYWORDS);
            InterestingIdentifierID = ID + FirstNotableIdentifier;
            assert(getNotableIdentifierID() == ID && "ID too large for field!");
        }

        unsigned getObjCOrBuiltinID() const { return InterestingIdentifierID; }
        void setObjCOrBuiltinID(unsigned ID) { InterestingIdentifierID = ID; }

        /// get/setExtension - Initialize information about whether or not this
        /// language token is an extension.  This controls extension warnings, and is
        /// only valid if a custom token ID is set.
        bool isExtensionToken() const { return IsExtension; }
        void setIsExtensionToken(bool Val) {
            IsExtension = Val;
            if (Val)
            NeedsHandleIdentifier = true;
            else
            RecomputeNeedsHandleIdentifier();
        }

        /// is/setIsFutureCompatKeyword - Initialize information about whether or not
        /// this language token is a keyword in a newer or proposed Standard. This
        /// controls compatibility warnings, and is only true when not parsing the
        /// corresponding Standard. Once a compatibility problem has been diagnosed
        /// with this keyword, the flag will be cleared.
        bool isFutureCompatKeyword() const { return IsFutureCompatKeyword; }
        void setIsFutureCompatKeyword(bool Val) {
            IsFutureCompatKeyword = Val;
            if (Val)
            NeedsHandleIdentifier = true;
            else
            RecomputeNeedsHandleIdentifier();
        }

        /// setIsPoisoned - Mark this identifier as poisoned.  After poisoning, the
        /// Preprocessor will emit an error every time this token is used.
        void setIsPoisoned(bool Value = true) {
            IsPoisoned = Value;
            if (Value)
            NeedsHandleIdentifier = true;
            else
            RecomputeNeedsHandleIdentifier();
        }

        /// Return true if this token has been poisoned.
        bool isPoisoned() const { return IsPoisoned; }

        /// isCPlusPlusOperatorKeyword/setIsCPlusPlusOperatorKeyword controls whether
        /// this identifier is a C++ alternate representation of an operator.
        void setIsCPlusPlusOperatorKeyword(bool Val = true) {
            IsCPPOperatorKeyword = Val;
        }
        bool isCPlusPlusOperatorKeyword() const { return IsCPPOperatorKeyword; }

        /// Return true if this token is a keyword in the specified language.
        bool isKeyword(const LangOptions &LangOpts) const;

        /// Return true if this token is a C++ keyword in the specified
        /// language.
        bool isCPlusPlusKeyword(const LangOptions &LangOpts) const;

        /// Get and set FETokenInfo. The language front-end is allowed to associate
        /// arbitrary metadata with this token.
        void *getFETokenInfo() const { return FETokenInfo; }
        void setFETokenInfo(void *T) { FETokenInfo = T; }

        /// Return true if the Preprocessor::HandleIdentifier must be called
        /// on a token of this identifier.
        ///
        /// If this returns false, we know that HandleIdentifier will not affect
        /// the token.
        bool isHandleIdentifierCase() const { return NeedsHandleIdentifier; }

        /// Return true if the identifier in its current state was loaded
        /// from an AST file.
        bool isFromAST() const { return IsFromAST; }

        void setIsFromAST() { IsFromAST = true; }

        /// Determine whether this identifier has changed since it was loaded
        /// from an AST file.
        bool hasChangedSinceDeserialization() const {
            return ChangedAfterLoad;
        }

        /// Note that this identifier has changed since it was loaded from
        /// an AST file.
        void setChangedSinceDeserialization() {
            ChangedAfterLoad = true;
        }

        /// Determine whether the frontend token information for this
        /// identifier has changed since it was loaded from an AST file.
        bool hasFETokenInfoChangedSinceDeserialization() const {
            return FEChangedAfterLoad;
        }

        /// Note that the frontend token information for this identifier has
        /// changed since it was loaded from an AST file.
        void setFETokenInfoChangedSinceDeserialization() {
            FEChangedAfterLoad = true;
        }

        /// Determine whether the information for this identifier is out of
        /// date with respect to the external source.
        bool isOutOfDate() const { return OutOfDate; }

        /// Set whether the information for this identifier is out of
        /// date with respect to the external source.
        void setOutOfDate(bool OOD) {
            OutOfDate = OOD;
            if (OOD)
            NeedsHandleIdentifier = true;
            else
            RecomputeNeedsHandleIdentifier();
        }

        /// Determine whether this is the contextual keyword \c import.
        bool isModulesImport() const { return IsModulesImport; }

        /// Set whether this identifier is the contextual keyword \c import.
        void setModulesImport(bool I) {
            IsModulesImport = I;
            if (I)
            NeedsHandleIdentifier = true;
            else
            RecomputeNeedsHandleIdentifier();
        }

        /// Determine whether this is the mangled name of an OpenMP variant.
        bool isMangledOpenMPVariantName() const { return IsMangledOpenMPVariantName; }

        /// Set whether this is the mangled name of an OpenMP variant.
        void setMangledOpenMPVariantName(bool I) { IsMangledOpenMPVariantName = I; }

        /// Return true if this identifier is an editor placeholder.
        ///
        /// Editor placeholders are produced by the code-completion engine and are
        /// represented as characters between '<#' and '#>' in the source code. An
        /// example of auto-completed call with a placeholder parameter is shown
        /// below:
        /// \code
        ///   function(<#int x#>);
        /// \endcode
        bool isEditorPlaceholder() const {
            return getName().starts_with("<#") && getName().ends_with("#>");
        }

        /// Determine whether \p this is a name reserved for the implementation (C99
        /// 7.1.3, C++ [lib.global.names]).
        ReservedIdentifierStatus isReserved(const LangOptions &LangOpts) const;

        /// Determine whether \p this is a name reserved for future standardization or
        /// the implementation (C++ [usrlit.suffix]).
        ReservedLiteralSuffixIdStatus isReservedLiteralSuffixId() const;

        /// If the identifier is an "uglified" reserved name, return a cleaned form.
        /// e.g. _Foo => Foo. Otherwise, just returns the name.
        StringRef deuglifiedName() const;
        bool isPlaceholder() const {
            return getLength() == 1 && getNameStart()[0] == '_';
        }

        /// Provide less than operator for lexicographical sorting.
        bool operator<(const IdentifierInfo &RHS) const {
            return getName() < RHS.getName();
        }

        private:
        /// The Preprocessor::HandleIdentifier does several special (but rare)
        /// things to identifiers of various sorts.  For example, it changes the
        /// \c for keyword token from tok::identifier to tok::for.
        ///
        /// This method is very tied to the definition of HandleIdentifier.  Any
        /// change to it should be reflected here.
        void RecomputeNeedsHandleIdentifier() {
            NeedsHandleIdentifier = isPoisoned() || hasMacroDefinition() ||
                                    isExtensionToken() || isFutureCompatKeyword() ||
                                    isOutOfDate() || isModulesImport();
        }
        };
    }

} 

#endif