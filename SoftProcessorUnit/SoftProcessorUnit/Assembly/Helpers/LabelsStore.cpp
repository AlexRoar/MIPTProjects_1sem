//
//  HashTable.cpp
//  SPUAsm
//
//  Created by Александр Дремов on 16.10.2020.
//

#include "LabelsStore.hpp"
#include <stdlib.h>
#include <string.h>


JMPLabel::JMPLabel(JMPLabel* previous, char* name) {
    if (previous != NULL)
        previous->next = this;
    this->nameLen = strlen(name);
    this->name = (char*)calloc(this->nameLen + 1, sizeof(char));
}

JMPLabel::~JMPLabel(){
    free(this->name);
}

void JMPLabel::wasUsed() {
    this->used++;
}

void JMPLabel::foundToPosition(unsigned int position) {
    this->positionTo = position;
}


void JMPLabelsStore::newLabel(char* label) {
    JMPLabel* newLabel = new JMPLabel(this->last, label);
    if (this->last == NULL)
        this->first = (this->last = newLabel);
    else
        this->last  = newLabel;
    
    
    JMPLabel* current = this->first;
    while (current != NULL && current != this->last) {
        if (strcmp(current->name, newLabel->name) == 0)
            throw "Label exists";
        
        current = current->next;
    }
}

void JMPLabelsStore::setLabelToPosition(char* label, unsigned int position) {
    JMPLabel* current = this->getLabel(label);
    
    if (current == NULL) {
        this->newLabel(label);
        current = this->last;
    }
    
    if (current->used != 0) {
        throw "This label was used";
    }
    current->wasUsed();
    current->positionTo = position;
}

void JMPLabelsStore::setLabelFromPosition(char* label, unsigned int position) {
    JMPLabel* current = this->getLabel(label);
    
    if (current == NULL) {
        this->newLabel(label);
        current = this->last;
    }
    current->positionFrom = position;
}

JMPLabel* JMPLabelsStore::getLabel(char* label) {
    JMPLabel* current = this->first;
    while (current != NULL && current != this->last) {
        if (strcmp(current->name, label) == 0) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}
