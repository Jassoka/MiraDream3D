//
// Created by jassoka on 6/3/26.
//

#include <gtest/gtest.h>
#include <filesystem>

#include "controller/AssetImporter.h"
#include "model/Scene.h"
#include "testUtils/MeshComparison.h"

namespace fs = std::filesystem;


TEST(ImporterTests, SmoothTangentsImportTest) {
    // =========================================================================
    // 1. ARRANGE (Préparation) : Construire le modèle mathématique "Attendu"
    // =========================================================================
    Mesh expectedMesh(0);

    // La valeur exacte de la normale lissée pour un coin de cube parfait
    const float n = 0.577350f;

    // 1. Ajout des sommets dans l'ordre exact du fichier
    expectedMesh.addVertex({ 1.0f,  1.0f,  1.0f,  n,  n,  n, 0.0f, 0.0f}); // Index 0 (v 1)
    expectedMesh.addVertex({ 1.0f,  1.0f, -1.0f,  n,  n, -n, 0.0f, 0.0f}); // Index 1 (v 2)
    expectedMesh.addVertex({ 1.0f, -1.0f,  1.0f,  n, -n,  n, 0.0f, 0.0f}); // Index 2 (v 3)
    expectedMesh.addVertex({ 1.0f, -1.0f, -1.0f,  n, -n, -n, 0.0f, 0.0f}); // Index 3 (v 4)
    expectedMesh.addVertex({-1.0f,  1.0f,  1.0f, -n,  n,  n, 0.0f, 0.0f}); // Index 4 (v 5)
    expectedMesh.addVertex({-1.0f,  1.0f, -1.0f, -n,  n, -n, 0.0f, 0.0f}); // Index 5 (v 6)
    expectedMesh.addVertex({-1.0f, -1.0f,  1.0f, -n, -n,  n, 0.0f, 0.0f}); // Index 6 (v 7)
    expectedMesh.addVertex({-1.0f, -1.0f, -1.0f, -n, -n, -n, 0.0f, 0.0f}); // Index 7 (v 8)

    // 2. Ajout des faces (Indices convertis : OBJ-1 = C++)
    // Face Avant
    expectedMesh.addFace({0, 4, 6}); // 1,5,7 -> 0,4,6
    expectedMesh.addFace({0, 6, 2}); // 1,7,3 -> 0,6,2

    // Face Arrière
    expectedMesh.addFace({5, 1, 3}); // 6,2,4 -> 5,1,3
    expectedMesh.addFace({5, 3, 7}); // 6,4,8 -> 5,3,7

    // Face Haut
    expectedMesh.addFace({4, 0, 1}); // 5,1,2 -> 4,0,1
    expectedMesh.addFace({4, 1, 5}); // 5,2,6 -> 4,1,5

    // Face Bas
    expectedMesh.addFace({6, 7, 3}); // 7,8,4 -> 6,7,3
    expectedMesh.addFace({6, 3, 2}); // 7,4,3 -> 6,3,2

    // Face Droite
    expectedMesh.addFace({0, 2, 3}); // 1,3,4 -> 0,2,3
    expectedMesh.addFace({0, 3, 1}); // 1,4,2 -> 0,3,1

    // Face Gauche
    expectedMesh.addFace({5, 7, 6}); // 6,8,7 -> 5,7,6
    expectedMesh.addFace({5, 6, 4}); // 6,7,5 -> 5,6,4
    // =========================================================================
    // 2. ACT (Exécution) : Importer le vrai fichier
    // =========================================================================
    fs::path cubePath = fs::path(TEST_ASSETS_DIR) / "testImport/cubes/cube_smoothNorm.obj";
    Scene importedScene;

    // On utilise ASSERT_TRUE pour stopper immédiatement le test si le fichier est introuvable
    ASSERT_TRUE(AssetImporter::loadAssimpScene(cubePath.string(), &importedScene))
        << cubePath.string() << " couldn't be read";

    // Sécurité : On s'assure que la scène n'est pas vide avant de lire le tableau
    ASSERT_EQ(importedScene.getMeshes().size(), 1)
        << "Erreur : La scène ne contient pas exactement 1 Mesh.";

    // On récupère le Mesh généré par votre moteur
    const Mesh& actualMesh = importedScene.getMeshes()[0];

    // =========================================================================
    // 3. ASSERT (Vérification) : La Comparaison Ultime
    // =========================================================================
    // C'est ici que la magie opère. Google Test va utiliser le `operator==`
    // des std::vector, qui va lui-même utiliser le `operator==` de vos Vertex !
    EXPECT_EQ(MeshComparison::canonicalMesh(actualMesh), MeshComparison::canonicalMesh(expectedMesh))
        << "Échec : Les données du Mesh importé ne correspondent pas au modèle mathématique parfait.";
}