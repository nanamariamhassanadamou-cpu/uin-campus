#include "uincampus/calculs.hpp"
#include "uincampus/regles.hpp"
#include "uincampus/saisie.hpp"
#include "uincampus/version.hpp"
#include <cstdlib>
#include <format>
#include <print>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace {

enum class Action { Continuer, QuitterOk, QuitterErreur };

void afficherLigne(std::string_view libelle, std::string_view valeur) {
    std::println("{:<24}{:>20}", libelle, valeur);
}

void afficherAide() {
    std::println("Usage : uin-campus [--version] [--aide]");
    std::println("  --version   Affiche la version du programme");
    std::println("  --aide      Affiche cette aide");
}

Action traiterOptions(std::span<char*> args) {
    if (args.size() <= 1) {
        return Action::Continuer;
    }
    const std::string_view option{args[1]};
    if (option == "--version") {
        std::println("uin-campus {}", uin::version());
        return Action::QuitterOk;
    }
    if (option == "--aide") {
        afficherAide();
        return Action::QuitterOk;
    }
    std::println(stderr, "Option inconnue : {}", option);
    afficherAide();
    return Action::QuitterErreur;
}

} // namespace

int main(int argc, char* argv[]) {
    const std::span<char*> args{argv, static_cast<std::size_t>(argc)};
    switch (traiterOptions(args)) {
        case Action::QuitterOk:
            return EXIT_SUCCESS;
        case Action::QuitterErreur:
            return EXIT_FAILURE;
        case Action::Continuer:
            break;
    }

    std::println("UIN-Campus {} — Relevé d'un étudiant\n", uin::version());
    const std::string nom = uin::lireTexte("Nom de l'étudiant : ");
    const int nbUE = uin::lireEntier("Nombre d'UE (1 à 30) : ", 1, 30);
    const auto tailleUE = static_cast<std::size_t>(nbUE);

    std::vector<double> notes;
    std::vector<double> coefficients;
    notes.reserve(tailleUE);
    coefficients.reserve(tailleUE);

    for (int i{1}; i <= nbUE; ++i) {
        notes.push_back(uin::lireNote(std::format("  Note de l'UE {} : ", i)));
        coefficients.push_back(
            static_cast<double>(uin::lireEntier(std::format("  Coefficient de l'UE {} : ", i), 1, 10)));
    }

    const double moyenneGenerale = uin::arrondir(uin::moyenne(notes, coefficients));
    const auto [mini, maxi, moyenneSimple, ecartType] = uin::calculerStatistiques(notes);
    const int validees = uin::compterValidees(notes);

    std::println("\n{:-<44}", "");
    afficherLigne("Étudiant", nom);
    afficherLigne("Moyenne pondérée", std::format("{:.2f}", moyenneGenerale));
    afficherLigne("Moyenne simple", std::format("{:.2f}", moyenneSimple));
    afficherLigne("Note min / max", std::format("{:.2f} / {:.2f}", mini, maxi));
    afficherLigne("Écart-type", std::format("{:.2f}", ecartType));
    afficherLigne("UE validées", std::format("{} / {}", validees, nbUE));
    afficherLigne("Mention", uin::libelle(uin::mentionPour(moyenneGenerale)));
    std::println("{:-<44}", "");

    return EXIT_SUCCESS;
}