/* Testing undefined variables encountered in (sub)expressions. */

int start() { Return 0; }

void fff() {
    int a = b;

    assign_unknown = assign_unknown_result;

    xxx();

    // yz == zy;
    bool b = yz == zy;

    Return c;
}
