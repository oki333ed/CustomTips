using namespace geode::prelude;

std::vector<std::string> splitTips(const std::string& str) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find('|');

    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find('|', start);
    }

    result.push_back(str.substr(start));
    return result;
}

std::string formatTip(std::string tip) {
    size_t pos;
    while ((pos = tip.find("/n")) != std::string::npos) {
        tip.replace(pos, 2, "\n");
    }
    return tip;
}

#include <Geode/modify/LoadingLayer.hpp>
class $modify(LoadingLayer) {
    bool init(bool refresh) {
        if (!LoadingLayer::init(refresh))
            return false;

        if (!Mod::get()->getSettingValue<bool>("enable"))
            return true;

        auto textArea = this->getChildByID("text-area");
        if (!textArea)
            return true;

        auto tipsStr = Mod::get()->getSettingValue<std::string>("tips");
        auto tipsRaw = splitTips(tipsStr);

        std::vector<std::string> tips;
        for (auto& t : tipsRaw) {
            if (!t.empty())
                tips.push_back(t);
        }

        if (tips.empty())
            return true;

        textArea->setVisible(false);

		auto pos = textArea->getPosition();
		auto scale = textArea->getScale();
		auto anchor = textArea->getAnchorPoint();
		auto size = textArea->getContentSize();

        int index = rand() % tips.size();
        std::string tip = formatTip(tips[index]);

        std::string fontSetting = Mod::get()->getSettingValue<std::string>("font");
        const char* font = "bigFont.fnt";

        if (fontSetting == "Gold Font") {
            font = "goldFont.fnt";
        }

        auto label = TextArea::create(
            tip.c_str(),
            font,
            1.0f,
            size.width,
            anchor,
            20.f,
            false
        );

        label->setPosition(pos);
        label->setScale(scale);
        label->setID("custom-text-area");

        this->addChild(label);

        return true;
    }
};