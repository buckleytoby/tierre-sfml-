// main-level events
// this way is worse I think, a series of nested if statements or switch-case statements
            switch(event){
                case sf::Event::Closed:
                    window.close();
                    continue;
                case sf::Event::KeyPressed:
                    switch(event.key.code){
                        case sf::Keyboard::Escape:
                            switch (gamescreenState){
                                case GamescreenStates::TITLE_SCREEN:
                                    window.close();
                                    continue;
                                case GamescreenStates::GAMEPLAY:
                                    gamescreenState = GamescreenStates::TITLE_SCREEN;
                                    activeGamescreen = gamescreen_map[gamescreenState];
                                    break;
                                default:
                                    break;
                            }
                            window.close();
                            continue;
                        default:
                            break;
                    }
            
            // active gamescreen takes next precedence
            auto flag = activeGamescreen->HandleInput(event);