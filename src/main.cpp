#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class $modify(ProfilePage) {

	bool init(int p0, bool p1) {
		if (!ProfilePage::init(p0, p1)) return false;

		std::string endpointUrl = "pointercrate.com/api/v1/players/ranking?name_contains=" + std::string(m_usernameLabel->getString());
		std::string apiKey = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpZCI6Njc0Nn0.7S3YnmDjkhEBONROQfONs5SVLTZIRWAwWdbmK2XmfcE";

		log::info("Getting info for {}", m_usernameLabel->getString());

		web::AsyncWebRequest()
		.header("Authorization", "Bearer " + apiKey)
		.header("Accept", "application/json")
		.method("GET")
		.fetch(endpointUrl)
		.json()
		.then([this](matjson::Value res) {
			if(res.dump() == "[]") {
				return log::info("Api didn't return info about user, skipping...");
			} else {
				auto topRes = res[0]["rank"].as_int();
				std::string topString = std::to_string(topRes);

				// Delete "my stuff" text
				m_mainLayer->removeChildByID("my-stuff-hint");

				// Badge sprite
				auto spr = CCSprite::create("demonlistBadge.png"_spr);
				spr->setPositionY(7.f);
				spr->setScale(1.1f);

				// Text
				auto label = CCLabelBMFont::create(topString.c_str(), "bigFont.fnt"); // ALSO HERE
				label->setPositionY(-18.f);
				// label->setPositionX(m_usernameLabel->getContentSize() - 2.f);
				label->setScale(0.65f);

				// Div kinda thingy
				auto node = CCNode::create();
				node->setLayout(ColumnLayout::create());
				node->setScale(0.6f);
				node->setPosition(394.f, 283.f);
				node->setZOrder(21);
				node->addChild(spr);
				node->addChild(label);
				m_mainLayer->addChild(node);
			}
		})
		.expect([](std::string const& err) {
			log::error("Error when making call to api, contact @elnexreal on Discord");
		});

		return true;
	}
};
