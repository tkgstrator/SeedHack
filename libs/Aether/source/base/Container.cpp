#include "Aether/base/Container.hpp"

namespace Aether {
    Container::Container(int x, int y, int w, int h) : Element(x, y, w, h) {

    }

    void Container::addElement(Element * e) {
        this->addElementAt(e, this->children.size());
    }

    void Container::addElementAt(Element * e, size_t i) {
        e->setInactive();
        Element::addElementAt(e, i);
        if ((e->selectable() || e->hasSelectable()) && this->focused() == nullptr) {
            this->setFocused(e);
        }
    }

    bool Container::handleEvent(InputEvent * e) {
        // Don't handle event if hidden!
        if (this->hidden()) {
            return false;
        }

        switch (e->type()) {
            case EventType::ButtonPressed:
                if (this->focused() == nullptr) {
                    return false;
                }
                // Default behaviour is to pass to focused
                if (this->focused()->handleEvent(e)) {
                    return true;
                }

                // If children didn't handle it, shift focus between them
                switch (e->button()) {
                    // Cur: current element that's focused
                    // Pot: potential element to move to
                    case Button::DPAD_RIGHT:
                        return moveHighlight(this, [](Element * cur, Element * pot) {
                            return (pot->x() >= cur->x() + cur->w());
                        },
                        [](Element * cur, Element * pot){
                            return sqrt(pow(pot->x() - (cur->x() + cur->w()), 2) + pow((pot->y() + pot->h()/2) - (cur->y() + cur->h()/2), 2));
                        });
                        break;

                    case Button::DPAD_LEFT:
                        return moveHighlight(this, [](Element * cur, Element * pot) {
                            return (pot->x() + pot->w() <= cur->x());
                        },
                        [](Element * cur, Element * pot){
                            return sqrt(pow((pot->x() + pot->w()) - cur->x(), 2) + pow((pot->y() + pot->h()/2) - (cur->y() + cur->h()/2), 2));
                        });
                        break;

                    case Button::DPAD_UP:
                        return moveHighlight(this, [](Element * cur, Element * pot) {
                            return (pot->y() + pot->h() <= cur->y());
                        },
                        [](Element * cur, Element * pot){
                            return sqrt(pow((pot->x() + pot->w()/2) - (cur->x() + cur->w()/2), 2) + pow((pot->y() + pot->h()) - cur->y(), 2));
                        });
                        break;

                    case Button::DPAD_DOWN:
                        return moveHighlight(this, [](Element * cur, Element * pot) {
                            return (pot->y() >= cur->y() + cur->h());
                        },
                        [](Element * cur, Element * pot){
                            return sqrt(pow((pot->x() + pot->w()/2) - (cur->x() + cur->w()/2), 2) + pow(pot->y() - (cur->y() + cur->h()), 2));
                        });
                        break;
                }
                break;

            case EventType::ButtonReleased:
                if (this->focused() != nullptr) {
                    if (this->focused()->handleEvent(e)) {
                        return true;
                    }
                }
                break;

            case EventType::TouchPressed:
            case EventType::TouchMoved:
            case EventType::TouchReleased:
                for (size_t i = 0; i < this->children.size(); i++) {
                    if (this->children[i]->handleEvent(e)) {
                        return true;
                    }
                }
                break;
        }

        return false;
    }

    void Container::removeAllElements() {
        this->setFocused(nullptr);
        Element::removeAllElements();
    }

    void Container::setActive() {
        if (this->focused() != nullptr) {
            this->focused()->setActive();
        }
    }

    void Container::setInactive() {
        if (this->focused() != nullptr) {
            this->focused()->setInactive();
        }
    }

    bool moveHighlight(Container * parent, std::function<bool(Element *, Element*)> check, std::function<int(Element *, Element *)> dist) {
        int minDist = std::numeric_limits<int>::max();
        Element * mv = nullptr;

        // Iterate over all children and keep valid one with smallest distance
        for (size_t i = 0; i < parent->children.size(); i++) {
            if (parent->children[i] == parent->focused() || parent->children[i]->hidden() || !(parent->children[i]->selectable() || parent->children[i]->hasSelectable())) {
                continue;
            }

            if (check(parent->focused(), parent->children[i])) {
                int tmpDist = dist(parent->focused(), parent->children[i]);
                if (tmpDist < minDist) {
                    minDist = tmpDist;
                    mv = parent->children[i];
                }
            }
        }

        // If one found change focus
        if (mv != nullptr) {
            parent->setFocused(mv);
            return true;
        }
        return false;
    }
};