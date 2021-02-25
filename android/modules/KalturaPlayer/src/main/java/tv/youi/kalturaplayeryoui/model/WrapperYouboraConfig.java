package tv.youi.kalturaplayeryoui.model;

import java.util.ArrayList;

public class WrapperYouboraConfig {

    private String accountCode;

    private String username;

    private String userEmail;

    private String userType;        // any string - free / paid etc.

    private Boolean parseManifest;

    private Boolean parseCdnNode;

    private Boolean parseCdnSwitchHeader;

    private ArrayList<String> cdnNodeList;

    private String cdnNameHeaders;

    private Integer parseCdnTTL;

    private String houseHoldId;    // which device is used to play

    private boolean httpSecure = true; // youbora events will be sent via https

    private String appName = "";

    private String appReleaseVersion = "";

    private String extraParam1;

    private String extraParam2;

    public WrapperYouboraConfig setAccountCode(String accountCode) {
        this.accountCode = accountCode;
        return this;
    }

    public String getAccountCode() {
        return accountCode;
    }

    public String getUsername() {
        return username;
    }

    public String getUserEmail() {
        return userEmail;
    }

    public String getUserType() {
        return userType;
    }

    public Boolean getParseManifest() {
        return parseManifest;
    }

    public Boolean getParseCdnNode() {
        return parseCdnNode;
    }

    public Boolean getParseCdnSwitchHeader() {
        return parseCdnSwitchHeader;
    }

    public ArrayList<String> getCdnNodeList() {
        return cdnNodeList;
    }

    public String getCdnNameHeaders() {
        return cdnNameHeaders;
    }

    public Integer getParseCdnTTL() {
        return parseCdnTTL;
    }

    public String getHouseHoldId() {
        return houseHoldId;
    }

    public boolean isHttpSecure() {
        return httpSecure;
    }

    public String getAppName() {
        return appName;
    }

    public String getAppReleaseVersion() {
        return appReleaseVersion;
    }

    public String getExtraParam1() {
        return extraParam1;
    }

    public String getExtraParam2() {
        return extraParam2;
    }
}

