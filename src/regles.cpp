Mention mentionPour(double moyenne) {
    int rang{0};
    for (double seuil : seuilsMentions) {
        if (moyenne >= seuil) {
            ++rang;
        }
    }
    return static_cast<Mention>(rang);
}