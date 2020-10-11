// Copyright 2020 Shift Cryptosecurity AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

use super::{confirm, status};
use crate::bb02_async::option;
use bitbox02::password::Password;
use core::cell::RefCell;

/// Example:
/// ```no_run
/// let mut pw = Password::new();
/// enter("Enter password", true, &mut pw).await;
/// // use pw.
/// ```
pub async fn enter(title: &str, special_chars: bool) -> Password {
    let result = RefCell::new(None);
    let mut component =
        bitbox02::ui::trinary_input_string_create_password(title, special_chars, |pw| {
            *result.borrow_mut() = Some(pw);
        });
    component.screen_stack_push();
    option(&result).await;
    drop(component);
    let result: Option<Password> = result.into_inner();
    result.unwrap()
}

/// Prompt the user to enter a password twice. A warning is displayed
/// if the password has fewer than 4 chars. Returns `Err` if the two
/// passwords do not match, or if the user aborts at the warning.
///
/// Example:
/// ```no_run
/// let pw = enter_twice().await.unwrap();
/// // use pw.
pub async fn enter_twice() -> Result<Password, ()> {
    let password = enter("Set password", false).await;
    let password_repeat = enter("Repeat password", false).await;
    if password.as_str() != password_repeat.as_str() {
        status::status("Passwords\ndo not match", false).await;
        return Err(());
    }
    if password.as_str().len() < 4 {
        let params = confirm::Params {
            title: "WARNING",
            body: "Your password\n has fewer than\n 4 characters.\nContinue?",
            longtouch: true,
            ..Default::default()
        };

        if !confirm::confirm(&params).await {
            return Err(());
        }
    }
    status::status("Success", true).await;
    Ok(password)
}
