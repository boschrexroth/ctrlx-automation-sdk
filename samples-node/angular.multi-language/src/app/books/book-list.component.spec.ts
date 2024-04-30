/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
import { ComponentFixture, TestBed } from '@angular/core/testing';
import { NO_ERRORS_SCHEMA } from '@angular/core';
import { RouterModule } from '@angular/router'
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { HttpClientTestingModule, HttpTestingController } from '@angular/common/http/testing';
import { TranslateModule, TranslateService } from '@ngx-translate/core';
import { ProductListComponent } from './book-list.component'


describe('ProductListComponent', () => {
  let fixture: ComponentFixture<ProductListComponent>;
  let component: ProductListComponent;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports:[
        BrowserAnimationsModule,
        HttpClientTestingModule,
        TranslateModule.forRoot(),
        RouterModule.forRoot([], { relativeLinkResolution: 'legacy' })
      ],
      declarations: [ ProductListComponent ],
      providers: [
        TranslateService,
      ],
      schemas:[ NO_ERRORS_SCHEMA ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ProductListComponent);
    component = fixture.componentInstance;

    fixture.detectChanges();
  });

  afterEach(() => {
    fixture.destroy();
  });

  describe('Component', ()=> {
  it('should create', () => {
    expect(component).toBeTruthy();
  });
});