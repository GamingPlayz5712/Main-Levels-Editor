#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/utils/web.hpp>
#include <ghc/filesystem.hpp>

using namespace geode::prelude;

//Popup do Fangame Creator com os campos de Input
class FangameSettingsPopup : public FLAlertLayer, public TextInputDelegate {
protected:
    CCTextInputNode* m_nameInput;
    CCTextInputNode* m_starsInput;
    CCTextInputNode* m_diffInput;

    bool init() override {
        if (!FLAlertLayer::init()) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        // Fundo da janela (Maior para caber os campos)
        auto bg = CCScale9Sprite::create("GJ_square01.png");
        bg->setContentSize({380, 260});
        bg->setPosition(winSize / 2);
        this->addChild(bg);

        // Titulo
        auto title = CCLabelBMFont::create("Fangame Creator", "bigFont.fnt");
        title->setPosition({winSize.width / 2, winSize.height / 2 + 110});
        title->setScale(0.7f);
        this->addChild(title);

        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        this->addChild(menu);

        // ---- CAMPO: NOME DA FASE ----
        auto nameBg = CCScale9Sprite::create("square02b_001.png");
        nameBg->setContentSize({200, 30});
        nameBg->setPosition({winSize.width / 2, winSize.height / 2 + 60});
        nameBg->setOpacity(100);
        this->addChild(nameBg);

        m_nameInput = CCTextInputNode::create(180, 20, "Level Name", "chatFont.fnt");
        m_nameInput->setPosition(nameBg->getPosition());
        m_nameInput->setDelegate(this);
        this->addChild(m_nameInput);

        // ---- CAMPO: ESTRELAS ----
        auto starsBg = CCScale9Sprite::create("square02b_001.png");
        starsBg->setContentSize({90, 30});
        starsBg->setPosition({winSize.width / 2 - 55, winSize.height / 2 + 15});
        starsBg->setOpacity(100);
        this->addChild(starsBg);

        m_starsInput = CCTextInputNode::create(80, 20, "Stars", "chatFont.fnt");
        m_starsInput->setPosition(starsBg->getPosition());
        m_starsInput->setDelegate(this);
        m_starsInput->setAllowedChars("0123456789"); // Apenas numeros
        this->addChild(m_starsInput);

        // ---- CAMPO: DIFICULDADE ----
        auto diffBg = CCScale9Sprite::create("square02b_001.png");
        diffBg->setContentSize({90, 30});
        diffBg->setPosition({winSize.width / 2 + 55, winSize.height / 2 + 15});
        diffBg->setOpacity(100);
        this->addChild(diffBg);

        m_diffInput = CCTextInputNode::create(80, 20, "Diff (1-6)", "chatFont.fnt");
        m_diffInput->setPosition(diffBg->getPosition());
        m_diffInput->setDelegate(this);
        m_diffInput->setAllowedChars("123456");
        this->addChild(m_diffInput);

        // Botao Salvar Dados Texto
        auto saveSprite = ButtonSprite::create("Save Info", "goldFont.fnt");
        saveSprite->setScale(0.8f);
        auto saveBtn = CCMenuItemSpriteExtra::create(
            saveSprite, this, menu_selector(FangameSettingsPopup::onSaveData)
        );
        saveBtn->setPosition({winSize.width / 2, winSize.height / 2 - 35});
        menu->addChild(saveBtn);

        // Botao Importar .txt (FilePicker)
        auto fileSprite = ButtonSprite::create("Import Level .txt", "bigFont.fnt");
        fileSprite->setScale(0.45f);
        auto fileBtn = CCMenuItemSpriteExtra::create(
            fileSprite, this, menu_selector(FangameSettingsPopup::onPickFile)
        );
        fileBtn->setPosition({winSize.width / 2, winSize.height / 2 - 75});
        menu->addChild(fileBtn);

        // Botao de Fechar (X no canto superior)
        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeBtn = CCMenuItemSpriteExtra::create(
            closeSprite, this, menu_selector(FangameSettingsPopup::onClose)
        );
        closeBtn->setPosition({winSize.width / 2 - 175, winSize.height / 2 + 115});
        menu->addChild(closeBtn);

        // Botao INFO
        auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        auto infoBtn = CCMenuItemSpriteExtra::create(
            infoSprite, this, menu_selector(FangameSettingsPopup::onInfo)
        );
        infoBtn->setPosition({winSize.width / 2 + 165, winSize.height / 2 + 110});
        menu->addChild(infoBtn);

        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        return true;
    }

    void onClose(CCObject*) {
        this->removeFromParentAndCleanup(true);
    }

    void onSaveData(CCObject*) {
        std::string name = m_nameInput->getString();
        std::string stars = m_starsInput->getString();
        std::string diff = m_diffInput->getString();

        // Salva as configuracoes de texto digitadas na pasta config do mod
        auto savePath = Mod::get()->getConfigDir() / "last_edited.txt";
        std::string data = "Name: " + name + "\nStars: " + stars + "\nDifficulty: " + diff;
        
        utils::file::writeString(savePath, data);
        FLAlertLayer::create("Saved", "Level data cached successfully!", "OK")->show();
    }

    void onInfo(CCObject*) {
        FLAlertLayer::create(
            "How to make a .txt file:",
            "- Open Geode mod loader\n"
            "- Download GDShare mod\n"
            "- Export the .gmd file\n"
            "- Go in ZArchiver (download from Play Store)\n"
            "- Open in text the .gmd folder\n"
            "- Copy all the text (this shows black screen in other phones)\n"
            "- Open the .txt file\n"
            "- Paste the text\n"
            "- Exit ZArchiver\n"
            "- Open geode\n"
            "- Import the .txt file",
            "OK"
        )->show();
    }

    void onPickFile(CCObject*) {
        file::FilePickOptions options;
        options.filters = { file::FileFilter("Text Files", { "*.txt" }) };
        
        file::pickFile(file::PickMode::OpenFile, options, [this](ghc::filesystem::path path) {
            auto destFolder = Mod::get()->getConfigDir() / "levels";
            auto destPath = destFolder / path.filename();

            try {
                ghc::filesystem::copy_file(path, destPath, ghc::filesystem::copy_options::overwrite_existing);
                FLAlertLayer::create("Success", "File imported to levels folder!", "OK")->show();
            } catch (std::exception& e) {
                FLAlertLayer::create("Error", "Failed to copy file.", "OK")->show();
            }
        });
    }

public:
    static FangameSettingsPopup* create() {
        auto ret = new FangameSettingsPopup();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// Criacao automatica das pastas ao carregar o mod
$execute {
    auto configDir = Mod::get()->getConfigDir();
    auto levelsDir = configDir / "levels";

    if (!ghc::filesystem::exists(levelsDir)) {
        ghc::filesystem::create_directories(levelsDir);
    }

    auto achFile = configDir / "achievements.json";
    if (!ghc::filesystem::exists(achFile)) {
        utils::file::writeString(achFile, "{\n  \"custom_achievements\": []\n}");
    }
}

// Botao na tela de informacoes do level (LevelInfoLayer)
class $modify(LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1)) return false;

        if (level->m_levelType == GJLevelType::Saved) return true;

        auto menu = this->getChildByID("right-side-menu");
        if (menu) {
            auto sprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
            sprite->setScale(0.8f);
            
            auto btn = CCMenuItemSpriteExtra::create(
                sprite, this, menu_selector(LevelInfoLayer::onFangameSettings)
            );
            
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }

    void onFangameSettings(CCObject*) {
        auto popup = FangameSettingsPopup::create();
        this->addChild(popup, 100);
    }
};

// Botao no menu de pausa (PauseLayer)
class $modify(PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto menu = this->getChildByID("right-side-menu");
        if (menu) {
            auto sprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
            sprite->setScale(0.8f);

            auto btn = CCMenuItemSpriteExtra::create(
                sprite, this, menu_selector(PauseLayer::onFangameSettings)
            );

            menu->addChild(btn);
            menu->updateLayout();
        }
    }

    void onFangameSettings(CCObject*) {
        auto popup = FangameSettingsPopup::create();
        this->addChild(popup, 100);
    }
};
