#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

const string CAT_RESERVED = "Reserve Word";
const string CAT_SYMBOL   = "Simbol/Tanda Baca";
const string CAT_VARIABLE = "Variabel";
const string CAT_NUMBER   = "Literal Angka";
const string CAT_STRING   = "Literal String";
const string CAT_CHAR     = "Literal Karakter";
const string CAT_COMMENT  = "Komentar";
const string CAT_UNKNOWN  = "Tidak Dikenal";

struct Token {
    string lexeme;
    string category;
    int line;
    int col;
};

struct MathSentence {
    string text;
    string reason;
    int line;
};

bool isIdentifierStart(char c) {
    unsigned char uc = static_cast<unsigned char>(c);
    return isalpha(uc) || c == '_';
}

bool isIdentifierPart(char c) {
    unsigned char uc = static_cast<unsigned char>(c);
    return isalnum(uc) || c == '_';
}

void advancePosition(char c, int &line, int &col) {
    if (c == '\n') {
        line++;
        col = 1;
    } else {
        col++;
    }
}

string displayLexeme(const string &s, size_t limit = 45) {
    string out;
    for (char c : s) {
        if (c == '\n') out += "\\n";
        else if (c == '\t') out += "\\t";
        else if (c == '\r') out += "\\r";
        else out += c;
    }

    if (out.size() > limit) {
        out = out.substr(0, limit - 3) + "...";
    }

    return out;
}

unordered_set<string> buildReservedWords() {
    return {
        "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool",
        "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t", "class",
        "compl", "concept", "const", "consteval", "constexpr", "constinit", "const_cast",
        "continue", "co_await", "co_return", "co_yield", "decltype", "default", "delete",
        "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern",
        "false", "float", "for", "friend", "goto", "if", "inline", "int", "long",
        "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator",
        "or", "or_eq", "private", "protected", "public", "register", "reinterpret_cast",
        "requires", "return", "short", "signed", "sizeof", "static", "static_assert",
        "static_cast", "struct", "switch", "template", "this", "thread_local", "throw",
        "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using",
        "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq",

        // Kata umum pada preprocessor.
        // Secara teknis bukan keyword C++, tetapi berguna untuk analisis source code C/C++.
        "include", "define", "ifdef", "ifndef", "endif", "elif", "else", "pragma", "undef"
    };
}

const unordered_set<string> RESERVED_WORDS = buildReservedWords();

const unordered_set<string> TYPE_WORDS = {
    "int", "float", "double", "char", "bool", "void", "long", "short",
    "signed", "unsigned", "auto", "string", "size_t", "long long",
    "wchar_t", "char16_t", "char32_t"
};

bool isTypeWord(const string &s) {
    return TYPE_WORDS.count(s) > 0;
}

vector<string> buildMultiCharOperators() {
    vector<string> ops = {
        ">>=", "<<=", "->*", "...", "::", "++", "--", "==", "!=", "<=", ">=",
        "&&", "||", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<", ">>",
        "->", "##", ".*"
    };

    sort(ops.begin(), ops.end(), [](const string &a, const string &b) {
        return a.size() > b.size();
    });

    return ops;
}

const vector<string> MULTI_CHAR_OPS = buildMultiCharOperators();

bool isSingleSymbol(char c) {
    static const string symbols = "{}[]();,.:?~#+-*/%=&|!<>^";
    return symbols.find(c) != string::npos;
}

vector<Token> tokenize(const string &source) {
    vector<Token> tokens;

    int line = 1;
    int col = 1;
    size_t i = 0;

    auto addToken = [&](const string &lexeme, const string &category, int startLine, int startCol) {
        tokens.push_back({lexeme, category, startLine, startCol});
    };

    while (i < source.size()) {
        char c = source[i];

        if (isspace(static_cast<unsigned char>(c))) {
            advancePosition(c, line, col);
            i++;
            continue;
        }

        int startLine = line;
        int startCol = col;

        // Komentar satu baris: // ...
        if (c == '/' && i + 1 < source.size() && source[i + 1] == '/') {
            string lexeme;

            while (i < source.size() && source[i] != '\n') {
                lexeme += source[i];
                advancePosition(source[i], line, col);
                i++;
            }

            addToken(lexeme, CAT_COMMENT, startLine, startCol);
            continue;
        }

        // Komentar blok: /* ... */
        if (c == '/' && i + 1 < source.size() && source[i + 1] == '*') {
            string lexeme;

            while (i < source.size()) {
                char cur = source[i];
                lexeme += cur;
                advancePosition(cur, line, col);
                i++;

                if (cur == '*' && i < source.size() && source[i] == '/') {
                    lexeme += source[i];
                    advancePosition(source[i], line, col);
                    i++;
                    break;
                }
            }

            addToken(lexeme, CAT_COMMENT, startLine, startCol);
            continue;
        }

        // String literal: "..."
        if (c == '"') {
            string lexeme;
            bool escape = false;

            while (i < source.size()) {
                char cur = source[i];
                lexeme += cur;
                advancePosition(cur, line, col);
                i++;

                if (escape) {
                    escape = false;
                } else if (cur == '\\') {
                    escape = true;
                } else if (cur == '"') {
                    break;
                }
            }

            addToken(lexeme, CAT_STRING, startLine, startCol);
            continue;
        }

        // Character literal: 'a', '\n', dan sebagainya.
        if (c == '\'') {
            string lexeme;
            bool escape = false;

            while (i < source.size()) {
                char cur = source[i];
                lexeme += cur;
                advancePosition(cur, line, col);
                i++;

                if (escape) {
                    escape = false;
                } else if (cur == '\\') {
                    escape = true;
                } else if (cur == '\'') {
                    break;
                }
            }

            addToken(lexeme, CAT_CHAR, startLine, startCol);
            continue;
        }

        // Number literal: integer, decimal, scientific notation, hexadecimal.
        if (isdigit(static_cast<unsigned char>(c)) ||
            (c == '.' && i + 1 < source.size() && isdigit(static_cast<unsigned char>(source[i + 1])))) {
            
            string lexeme;

            if (c == '0' && i + 1 < source.size() && (source[i + 1] == 'x' || source[i + 1] == 'X')) {
                lexeme += source[i];
                advancePosition(source[i], line, col);
                i++;

                lexeme += source[i];
                advancePosition(source[i], line, col);
                i++;

                while (i < source.size() && isxdigit(static_cast<unsigned char>(source[i]))) {
                    lexeme += source[i];
                    advancePosition(source[i], line, col);
                    i++;
                }
            } else {
                bool hasDot = false;

                while (i < source.size()) {
                    char cur = source[i];

                    if (isdigit(static_cast<unsigned char>(cur))) {
                        lexeme += cur;
                        advancePosition(cur, line, col);
                        i++;
                    } else if (cur == '.' && !hasDot) {
                        hasDot = true;
                        lexeme += cur;
                        advancePosition(cur, line, col);
                        i++;
                    } else {
                        break;
                    }
                }

                if (i < source.size() && (source[i] == 'e' || source[i] == 'E')) {
                    size_t expPos = i;
                    string expPart;

                    expPart += source[i];
                    i++;

                    if (i < source.size() && (source[i] == '+' || source[i] == '-')) {
                        expPart += source[i];
                        i++;
                    }

                    bool hasExpDigit = false;

                    while (i < source.size() && isdigit(static_cast<unsigned char>(source[i]))) {
                        hasExpDigit = true;
                        expPart += source[i];
                        i++;
                    }

                    if (hasExpDigit) {
                        for (char ec : expPart) {
                            lexeme += ec;
                            advancePosition(ec, line, col);
                        }
                    } else {
                        i = expPos;
                    }
                }
            }

            while (i < source.size() &&
                   (isalpha(static_cast<unsigned char>(source[i])) || source[i] == '_')) {
                lexeme += source[i];
                advancePosition(source[i], line, col);
                i++;
            }

            addToken(lexeme, CAT_NUMBER, startLine, startCol);
            continue;
        }

        // Identifier atau reserve word.
        if (isIdentifierStart(c)) {
            string lexeme;

            while (i < source.size() && isIdentifierPart(source[i])) {
                lexeme += source[i];
                advancePosition(source[i], line, col);
                i++;
            }

            if (RESERVED_WORDS.count(lexeme)) {
                addToken(lexeme, CAT_RESERVED, startLine, startCol);
            } else {
                addToken(lexeme, CAT_VARIABLE, startLine, startCol);
            }

            continue;
        }

        // Operator multi-karakter dengan prinsip longest match.
        bool matched = false;

        for (const string &op : MULTI_CHAR_OPS) {
            if (i + op.size() <= source.size() && source.compare(i, op.size(), op) == 0) {
                for (char oc : op) {
                    advancePosition(oc, line, col);
                }

                addToken(op, CAT_SYMBOL, startLine, startCol);
                i += op.size();
                matched = true;
                break;
            }
        }

        if (matched) {
            continue;
        }

        // Simbol satu karakter.
        if (isSingleSymbol(c)) {
            string lexeme(1, c);
            advancePosition(c, line, col);
            i++;

            addToken(lexeme, CAT_SYMBOL, startLine, startCol);
            continue;
        }

        // Karakter yang tidak dikenali.
        string lexeme(1, c);
        advancePosition(c, line, col);
        i++;

        addToken(lexeme, CAT_UNKNOWN, startLine, startCol);
    }

    return tokens;
}

string joinTokenLexemes(const vector<Token> &tokens) {
    string result;
    string prev;

    auto noSpaceBefore = [](const string &x) {
        static const unordered_set<string> s = {
            ")", "]", "}", ";", ",", ".", "::", "->"
        };
        return s.count(x) > 0;
    };

    auto noSpaceAfter = [](const string &x) {
        static const unordered_set<string> s = {
            "(", "[", "{", ".", "::", "->"
        };
        return s.count(x) > 0;
    };

    for (const Token &t : tokens) {
        string cur = t.lexeme;

        if (result.empty()) {
            result += cur;
        } else if (noSpaceBefore(cur) || noSpaceAfter(prev)) {
            result += cur;
        } else {
            result += " " + cur;
        }

        prev = cur;
    }

    return result;
}

bool statementStartsWithPreprocessor(const vector<Token> &st) {
    return !st.empty() && st[0].lexeme == "#";
}

bool looksLikeSimpleDeclaration(const vector<Token> &st) {
    if (st.empty()) return false;
    if (!isTypeWord(st[0].lexeme)) return false;

    static const unordered_set<string> forbidden = {
        "=", "+=", "-=", "*=", "/=", "%=", "==", "!=", "<", ">", "<=", ">=",
        "+", "-", "/", "%", "++", "--"
    };

    for (const Token &t : st) {
        if (forbidden.count(t.lexeme)) {
            return false;
        }
    }

    return true;
}

bool isFunctionCallAt(const vector<Token> &st, size_t i) {
    if (i + 1 >= st.size()) return false;
    if (st[i].category != CAT_VARIABLE) return false;
    if (st[i + 1].lexeme != "(") return false;

    // Jika sebelumnya tipe data, kemungkinan besar ini deklarasi fungsi.
    // Contoh: int main()
    if (i > 0 && isTypeWord(st[i - 1].lexeme)) {
        return false;
    }

    return true;
}

string classifyMathStatement(const vector<Token> &st) {
    if (st.empty()) return "";
    if (statementStartsWithPreprocessor(st)) return "";
    if (looksLikeSimpleDeclaration(st)) return "";

    static const unordered_set<string> assignOps = {
        "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", ">>=", "<<="
    };

    static const unordered_set<string> arithmeticOps = {
        "+", "-", "*", "/", "%", "^", "++", "--"
    };

    static const unordered_set<string> comparisonOps = {
        "==", "!=", "<", ">", "<=", ">=", "&&", "||"
    };

    static const unordered_set<string> mathFunctions = {
        "sin", "cos", "tan", "asin", "acos", "atan", "sqrt", "pow",
        "log", "log10", "exp", "abs", "fabs", "ceil", "floor",
        "round", "min", "max", "gcd", "lcm", "fmod"
    };

    bool hasAssignment = false;
    bool hasArithmetic = false;
    bool hasComparison = false;
    bool hasFunctionCall = false;
    bool hasKnownMathFunction = false;

    for (size_t i = 0; i < st.size(); i++) {
        const string &x = st[i].lexeme;

        if (assignOps.count(x)) hasAssignment = true;
        if (arithmeticOps.count(x)) hasArithmetic = true;
        if (comparisonOps.count(x)) hasComparison = true;

        if (isFunctionCallAt(st, i)) {
            hasFunctionCall = true;

            if (mathFunctions.count(st[i].lexeme)) {
                hasKnownMathFunction = true;
            }
        }
    }

    if (!(hasAssignment || hasArithmetic || hasComparison || hasFunctionCall || hasKnownMathFunction)) {
        return "";
    }

    vector<string> reasons;

    if (hasAssignment) reasons.push_back("persamaan/assignment");
    if (hasArithmetic) reasons.push_back("ekspresi aritmetika");
    if (hasComparison) reasons.push_back("perbandingan/logika");

    if (hasKnownMathFunction) {
        reasons.push_back("fungsi matematika");
    } else if (hasFunctionCall) {
        reasons.push_back("fungsi/pemanggilan fungsi");
    }

    string reason;

    for (size_t i = 0; i < reasons.size(); i++) {
        if (i) reason += ", ";
        reason += reasons[i];
    }

    return reason;
}

vector<MathSentence> extractMathSentences(const vector<Token> &tokens) {
    vector<MathSentence> result;
    vector<Token> current;

    int parenDepth = 0;

    auto flush = [&]() {
        if (current.empty()) return;

        string reason = classifyMathStatement(current);

        if (!reason.empty()) {
            result.push_back({
                joinTokenLexemes(current),
                reason,
                current.front().line
            });
        }

        current.clear();
    };

    for (const Token &t : tokens) {
        if (t.category == CAT_COMMENT) {
            continue;
        }

        current.push_back(t);

        if (t.lexeme == "(") {
            parenDepth++;
        } else if (t.lexeme == ")" && parenDepth > 0) {
            parenDepth--;
        }

        if (parenDepth == 0 &&
            (t.lexeme == ";" || t.lexeme == "{" || t.lexeme == "}")) {
            flush();
        }
    }

    flush();

    return result;
}

void printSetLine(ostream &out, const string &title, const set<string> &values) {
    out << "\n" << title << " (" << values.size() << "):\n";

    if (values.empty()) {
        out << "  -\n";
        return;
    }

    int count = 0;

    for (const string &v : values) {
        out << "  " << left << setw(18) << displayLexeme(v, 18);
        count++;

        if (count % 4 == 0) {
            out << "\n";
        }
    }

    if (count % 4 != 0) {
        out << "\n";
    }
}

void printReport(
    ostream &out,
    const vector<Token> &tokens,
    const vector<MathSentence> &mathSentences
) {
    out << "\n================ HASIL ANALISIS TOKEN ================\n";

    out << left
        << setw(5)  << "No"
        << setw(48) << "Token/String"
        << setw(22) << "Kelompok"
        << setw(8)  << "Baris"
        << setw(8)  << "Kolom"
        << "\n";

    out << string(91, '-') << "\n";

    for (size_t i = 0; i < tokens.size(); i++) {
        out << left
            << setw(5)  << (i + 1)
            << setw(48) << displayLexeme(tokens[i].lexeme)
            << setw(22) << tokens[i].category
            << setw(8)  << tokens[i].line
            << setw(8)  << tokens[i].col
            << "\n";
    }

    set<string> reserved;
    set<string> symbols;
    set<string> variables;
    set<string> numbers;
    set<string> strings;
    set<string> chars;
    set<string> comments;
    set<string> unknowns;

    for (const Token &t : tokens) {
        if (t.category == CAT_RESERVED) {
            reserved.insert(t.lexeme);
        } else if (t.category == CAT_SYMBOL) {
            symbols.insert(t.lexeme);
        } else if (t.category == CAT_VARIABLE) {
            variables.insert(t.lexeme);
        } else if (t.category == CAT_NUMBER) {
            numbers.insert(t.lexeme);
        } else if (t.category == CAT_STRING) {
            strings.insert(t.lexeme);
        } else if (t.category == CAT_CHAR) {
            chars.insert(t.lexeme);
        } else if (t.category == CAT_COMMENT) {
            comments.insert(t.lexeme);
        } else {
            unknowns.insert(t.lexeme);
        }
    }

    out << "\n================ RINGKASAN PER KELOMPOK ================\n";

    printSetLine(out, "Reserve words", reserved);
    printSetLine(out, "Simbol dan tanda baca", symbols);
    printSetLine(out, "Variabel/identifier", variables);
    printSetLine(out, "Literal angka", numbers);
    printSetLine(out, "Literal string", strings);
    printSetLine(out, "Literal karakter", chars);
    printSetLine(out, "Komentar", comments);
    printSetLine(out, "Token tidak dikenal", unknowns);

    out << "\n================ KALIMAT MATEMATIKA ================\n";

    if (mathSentences.empty()) {
        out << "Tidak ditemukan kalimat matematika.\n";
    } else {
        for (size_t i = 0; i < mathSentences.size(); i++) {
            out << i + 1 << ". Baris " << mathSentences[i].line
                << " | " << mathSentences[i].reason << "\n";
            out << "   " << mathSentences[i].text << "\n";
        }
    }

    out << "======================================================\n";
}

string readFileToString(const string &path) {
    ifstream fin(path);

    if (!fin) {
        return "";
    }

    stringstream buffer;
    buffer << fin.rdbuf();

    return buffer.str();
}

void printMenu() {
    cout << "\n========== TOKENIZER PROGRAM C++ ==========\n";
    cout << "1. Input program secara manual\n";
    cout << "2. Baca program dari file\n";
    cout << "3. Tampilkan source program saat ini\n";
    cout << "4. Analisis token\n";
    cout << "5. Simpan hasil analisis ke file\n";
    cout << "0. Keluar\n";
    cout << "Pilih menu: ";
}

int main() {
    string sourceCode;

    vector<Token> lastTokens;
    vector<MathSentence> lastMath;

    bool alreadyAnalyzed = false;

    while (true) {
        printMenu();

        int choice;

        if (!(cin >> choice)) {
            cout << "Input menu tidak valid. Program dihentikan.\n";
            return 0;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) {
            cout << "Program selesai.\n";
            break;
        }

        if (choice == 1) {
            cout << "\nMasukkan source code.\n";
            cout << "Ketik ###END### pada baris baru untuk selesai.\n";

            string line;
            string temp;

            while (getline(cin, line)) {
                if (line == "###END###") {
                    break;
                }

                temp += line + "\n";
            }

            sourceCode = temp;
            alreadyAnalyzed = false;

            cout << "Source code berhasil disimpan di memori.\n";
        } else if (choice == 2) {
            cout << "Masukkan path file source code: ";

            string path;
            getline(cin, path);

            string content = readFileToString(path);

            if (content.empty()) {
                cout << "File tidak dapat dibaca atau file kosong.\n";
            } else {
                sourceCode = content;
                alreadyAnalyzed = false;

                cout << "File berhasil dibaca. Panjang source: "
                     << sourceCode.size()
                     << " karakter.\n";
            }
        } else if (choice == 3) {
            if (sourceCode.empty()) {
                cout << "Belum ada source code. Gunakan menu 1 atau 2.\n";
            } else {
                cout << "\n================ SOURCE CODE ================\n";
                cout << sourceCode;
                cout << "=============================================\n";
            }
        } else if (choice == 4) {
            if (sourceCode.empty()) {
                cout << "Belum ada source code. Gunakan menu 1 atau 2.\n";
            } else {
                lastTokens = tokenize(sourceCode);
                lastMath = extractMathSentences(lastTokens);
                alreadyAnalyzed = true;

                printReport(cout, lastTokens, lastMath);
            }
        } else if (choice == 5) {
            if (sourceCode.empty()) {
                cout << "Belum ada source code. Gunakan menu 1 atau 2.\n";
                continue;
            }

            if (!alreadyAnalyzed) {
                lastTokens = tokenize(sourceCode);
                lastMath = extractMathSentences(lastTokens);
                alreadyAnalyzed = true;
            }

            cout << "Masukkan nama file output, contoh hasil_token.txt: ";

            string outPath;
            getline(cin, outPath);

            ofstream fout(outPath);

            if (!fout) {
                cout << "Gagal membuat file output.\n";
            } else {
                printReport(fout, lastTokens, lastMath);
                cout << "Hasil analisis berhasil disimpan ke " << outPath << "\n";
            }
        } else {
            cout << "Pilihan tidak tersedia.\n";
        }
    }

    return 0;
}